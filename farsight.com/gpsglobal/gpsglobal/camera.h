


#pragma once
#ifndef __CAMERA_H__
#define __CAMERA_H__

#include <linux/videodev.h>
#include <pthread.h>

class CCamera
{

public:
        CCamera(const char * devfile, int buffercount);
        ~CCamera();


        const char *    GetImageType();
        int             GetImageFileSize();
        int             GetImageWidth();
        int             GetImageHeight();
        const void *    GetBuffers();

        void            Pause();
        void            Resume();

private:
        void init(const char * devfile, int buffercount);

        struct v4l2_format              format;
        struct v4l2_requestbuffers      req;
        struct v4l2_buffer              buf;
        struct buffer
        {
                void *          start;
                unsigned int    length;
        } *buffers;

        int             index;
        int             fd;
        char            imagetype[5];
        bool            run;
        bool            isPause;

        static void * capture_thread(void * param);
        pthread_t       pid;

        char            dev[1024];
        int             buffercount;

};


#endif



