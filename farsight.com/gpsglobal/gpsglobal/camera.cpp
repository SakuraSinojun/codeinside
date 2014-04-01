
#include "camera.h"
#include <string.h>
#include <fcntl.h>
#include <linux/videodev.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <sys/mman.h>
#include "exception.h"
#include <sys/ioctl.h>
#include <pthread.h>
#include <QDebug>
#include <unistd.h>


void * CCamera::capture_thread(void *param)
{
        CCamera *       pThis = (CCamera *)param;
        int             i;

        i = 0;
        while(pThis->run)
        {
                while(pThis->isPause)
                {
                        usleep(100000);
                }
                memset(&pThis->buf, 0, sizeof(pThis->buf));
                pThis->buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
                pThis->buf.memory = V4L2_MEMORY_MMAP;
                pThis->buf.index = i;

                if(-1 == ioctl(pThis->fd, VIDIOC_DQBUF, &pThis->buf))
                {
                        perror("VIDIOC_DQBUF");
                        ioctl(pThis->fd, VIDIOC_QBUF, &pThis->buf);
                        return NULL;
                }
                if(-1 == ioctl(pThis->fd, VIDIOC_QBUF, &pThis->buf))
                {
                        perror("VIDIOC_QBUF");
                        return NULL;
                }

                i = (i+1) % pThis->req.count;
        }

        return NULL;
}

void CCamera::init(const char *devfile, int buffercount)
{

        index = 0;
        fd = open(devfile, O_RDONLY);
        if(fd == -1)
        {
                perror("open");
                throw CGPSException(errno, "open");
                return;
        }

        memset(&format, 0, sizeof(format));
        format.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
        if(-1 == ioctl(fd, VIDIOC_G_FMT, &format))
        {
                perror("VIDIOC_G_FMT");
                throw CGPSException(errno, "VIDIOC_G_FMT");
                return;
        }

        memset(imagetype, 0, sizeof(imagetype));
        imagetype[0] = (char)((format.fmt.pix.pixelformat >> 0 ) & 0xff);
        imagetype[1] = (char)((format.fmt.pix.pixelformat >> 8 ) & 0xff);
        imagetype[2] = (char)((format.fmt.pix.pixelformat >>16 ) & 0xff);
        imagetype[3] = (char)((format.fmt.pix.pixelformat >>24 ) & 0xff);
        imagetype[4] = '\0';


        qDebug("width: %d\n"
                "height: %d\n"
                "pixelformat: %s\n"
                "field: %d\n"
                "bytesperline: %d\n"
                "sizeimage: %d\n"
                "colorspace: %d\n"
                "priv: %d\n",
                format.fmt.pix.width,
                format.fmt.pix.height,
                imagetype,
                format.fmt.pix.field,
                format.fmt.pix.bytesperline,
                format.fmt.pix.sizeimage,
                format.fmt.pix.colorspace,
                format.fmt.pix.priv);


        memset(&req, 0, sizeof(req));
        req.count = buffercount;
        req.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
        req.memory = V4L2_MEMORY_MMAP;
        if(-1 == ioctl(fd, VIDIOC_REQBUFS, &req))
        {
                perror("VIDIOC_REQBUFS");
                throw CGPSException(errno, "VIDIOC_REQBUFS");
                return;
        }

        qDebug("count: %d\n"
                "type: %d\n"
                "memory: %d\n",
                req.count,
                req.type,
                req.memory);


        buffers = (struct buffer *)malloc(sizeof(struct buffer) * req.count);
        unsigned int     i;
        for(i=0; i<req.count; i++)
        {
                memset(&buf, 0, sizeof(buf));
                buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
                buf.memory = V4L2_MEMORY_MMAP;
                buf.index = i;
                if(-1 == ioctl(fd, VIDIOC_QUERYBUF, &buf))
                {
                        perror("VIDIOC_QUERYBUF");
                        return;
                }
                buffers[i].length = buf.length;
                buffers[i].start = mmap(NULL, buf.length, PROT_READ, MAP_SHARED, fd, buf.m.offset);

                if(MAP_FAILED == buffers[i].start)
                {
                        if(i == 0)
                        {
                                perror("mmap");
                                throw CGPSException(errno, "mmap");
                                return;
                        }
                        else
                        {
                                req.count = i;
                                break;
                        }
                }

                if(ioctl(fd, VIDIOC_QBUF, &buf) == -1)
                {
                        perror("VIDIOC_QBUF");
                        throw CGPSException(errno, "VIDIOC_QBUF");
                        return;
                }
        }

        enum v4l2_buf_type      type;
        type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
        if(-1 == ioctl(fd, VIDIOC_STREAMON, &type))
        {
                perror("VIDIOC_STREAMON");
                throw CGPSException(errno, "VIDIOC_STREAMON");
                return;
        }

}

CCamera::CCamera(const char * devfile, int buffercount)
{    
        strncpy(this->dev, devfile, 1024);
        this->buffercount = buffercount;

        try
        {
                this->init(dev, buffercount);
        }
        catch(CGPSException& e)
        {
                throw e;
        }

        run = true;
        isPause = false;
        if(pthread_create(&pid, NULL, capture_thread, this) != 0)
        {
                perror("pthread_create");
                throw CGPSException(errno, "pthread_create");
                return;
        }

}

CCamera::~CCamera()
{
        this->isPause = false;
        this->run = false;
        pthread_join(this->pid, NULL);
        qDebug() << "thread terminated.";

        free(this->buffers);
        close(this->fd);
        qDebug() << "dev file closed.";

}

const char * CCamera::GetImageType()
{
        return imagetype;
}

int CCamera::GetImageWidth()
{
        return format.fmt.pix.width;
}

int CCamera::GetImageHeight()
{
        return format.fmt.pix.height;
}

int CCamera::GetImageFileSize()
{
        //return this->buffers[index].length;
        return format.fmt.pix.sizeimage;
}

const void * CCamera::GetBuffers()
{
        void *          p;

        p = buffers[index].start;

        index = (index + 1) % req.count;

        return p;
}

void CCamera::Pause()
{
        isPause = true;
}

void CCamera::Resume()
{
        isPause = false;
}


