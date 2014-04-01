
#include "Surface.h"
#include "type.h"
#include "Render.h"
#include <algorithm>
#include <cstring>
#include <fstream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <iostream>
#include "jpeg/jpeglib.h"
#include "png/png.h"

#include "log.h"

DECLARE_MODULE(render);

// 解析PNG用
typedef struct
{
        const unsigned char *   buffer;
        unsigned int            pos;
        unsigned int            length;
}BUF;

static void png_read_data(png_structp png_ptr, png_bytep data, png_size_t length)
{
        BUF *           p = (BUF *)png_get_io_ptr(png_ptr);
        png_size_t      len;
        if(p != NULL)
        {
                len = length;
                if(p->length - p->pos < len)
                        len = p->length - p->pos;
                memcpy(data, p->buffer + p->pos, len);
                p->pos += len;
        }
}


CSurface::CSurface():
        nWidth(640),
        nHeight(480),
        nSrcLeft(0),
        nSrcTop(0),
        bColorKey(false),
        bUseFirstColor(false),
        uKeyColor(0),
        nAlpha(-1),
        bShow(false),
        imgbuffer(NULL),
        render(NULL),
        x(0),
        y(0),
        ox(0),
        oy(0)
{
}

CSurface::CSurface(int width, int height):
        nWidth(width),
        nHeight(height),
        nSrcLeft(0),
        nSrcTop(0),
        bColorKey(false),
        bUseFirstColor(false),
        uKeyColor(0),
        nAlpha(-1),
        bShow(false),
        imgbuffer(NULL),
        render(NULL),
        x(0),
        y(0),
        ox(0),
        oy(0)
{
}

CSurface::~CSurface()
{
        if(imgbuffer != NULL)
                delete [] imgbuffer;
}
      
int     CSurface::AttachTo(CRender* render)
{
        if(render == NULL)
                return 0;
        render->Attach(this);
        this->render = render;
        return 0;
}

int     CSurface::Detach(void)
{
        if(this->render == NULL)
                return 0;
        this->render->Detach(this);
        return 0;
}

int CSurface::ConvertPNG(const unsigned char * imgbuf, int len)
{/*{{{*/
        png_structp     png_ptr;
        png_infop       info_ptr, end_info_ptr;

        png_ptr         = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
        info_ptr        = png_create_info_struct(png_ptr);
        end_info_ptr    = png_create_info_struct(png_ptr);

        if(setjmp(png_jmpbuf(png_ptr)))
        {
                Debug() << "read png error.";
                png_destroy_read_struct(&png_ptr, &info_ptr, &end_info_ptr);
                return -1;
        }
        BUF     buf;
        buf.buffer      = imgbuf;
        buf.pos         = 0;
        buf.length      = len;
        png_set_read_fn(png_ptr, (png_voidp)&buf, png_read_data);
        png_read_png(png_ptr, info_ptr, PNG_TRANSFORM_BGR, NULL);
        
        png_uint_32     width, height;
        int             depth, color_type;
        int             channels;

        width           = png_get_image_width(png_ptr, info_ptr);
        height          = png_get_image_height(png_ptr, info_ptr);
        depth           = png_get_bit_depth(png_ptr, info_ptr);
        color_type      = png_get_color_type(png_ptr, info_ptr);
        channels        = png_get_channels(png_ptr, info_ptr);
        // rowbytes        = png_get_rowbytes(png_ptr, info_ptr);

        if(!(color_type & PNG_COLOR_TYPE_RGB))
        {
                Debug() << "懒得转换的PNG颜色类型。。请用RGB。。";
                return -1;
        }
        if(depth != 8)
        {
                Debug() << "非8bit/色位图。。。懒得转。";
                return -1;
        }
        if(channels != 3 && channels != 4)
        {
                Debug() << "非24/32位位图。。。懒得转。";
                return -1;
        }

        render->Lock();
        imgbuffer = new unsigned char [width * height * 4];
        png_bytepp      bf;
        bf = png_get_rows(png_ptr, info_ptr);
        unsigned int     i, j, l;
        int     r, g, b, a;
        int     bytes_per_line = width * 4;

        if(channels == 3 || channels == 4)
        {
                for(j=0; j<height; j++)
                {
                        for(i=0,l=0; i<width*channels; i+=channels, l+=4)
                        {
                                b = bf[j][i + 0];
                                g = bf[j][i + 1];
                                r = bf[j][i + 2];
                                a = (r + b + g) / 3;
                                
                                imgbuffer[(height - j - 1) * bytes_per_line + l + 0] = b;
                                imgbuffer[(height - j - 1) * bytes_per_line + l + 1] = g;
                                imgbuffer[(height - j - 1) * bytes_per_line + l + 2] = r;
                                imgbuffer[(height - j - 1) * bytes_per_line + l + 3] = a;
                        }
                }
        }

        int offset = sizeof(BMPHEADER) + sizeof(BMPINFOHEADER);
        memset(&bmpHeader, 0, sizeof(BMPHEADER));
        memset(&biHeader, 0, sizeof(BMPINFOHEADER));
        bmpHeader.bfType        = 0x4d42;
        bmpHeader.bfSize        = offset + bytes_per_line * height;
        bmpHeader.bfOffBits     = offset;
        biHeader.biSize         = sizeof(BMPINFOHEADER);
        biHeader.biWidth        = width;
        biHeader.biHeight       = height;
        biHeader.biPlanes       = 0;
        biHeader.biBitCount     = 32;
        biHeader.biCompression  = 0;
        biHeader.biSizeImage    = bytes_per_line * height;
        biHeader.biXPelsPerMeter = 3780;
        biHeader.biYPelsPerMeter = 3780;
        nWidth = width;
        nHeight = height;

        png_destroy_read_struct(&png_ptr, &info_ptr, &end_info_ptr);

        render->Unlock();
        return 0;
}/*}}}*/

int CSurface::ConvertJPG(const unsigned char * imgbuf, int len)
{/*{{{*/
        struct jpeg_decompress_struct   cinfo;
        struct jpeg_error_mgr           jerr;
        JSAMPROW                        row_pointer;
        int                             w, h, d, bytes_per_line, offset;
        unsigned char *                 buffer;
        
        render->Lock();

        cinfo.err = jpeg_std_error(&jerr);
        jpeg_create_decompress(&cinfo);
        jpeg_stdio_src(&cinfo, (FILE *)imgbuf);
        jpeg_set_buffer_len(len);
        jpeg_read_header(&cinfo, 1);

        w = cinfo.image_width;
        h = cinfo.image_height;
        d = cinfo.num_components;
        bytes_per_line = w * 4;
        while((bytes_per_line & 3) != 0)
                bytes_per_line ++;

        offset = sizeof(BMPHEADER) + sizeof(BMPINFOHEADER);
        imgbuffer = new unsigned char [bytes_per_line * h];
        buffer = new unsigned char [w * d * h];
        memset(buffer, 0, w * d * h);
        
        jpeg_start_decompress(&cinfo);
        while(cinfo.output_scanline < cinfo.output_height)
        {
                row_pointer = &buffer[cinfo.output_scanline * w * d];
                jpeg_read_scanlines(&cinfo, &row_pointer, 1);
        }
        jpeg_destroy_decompress(&cinfo);

        int     i, j, l;
        int     r, g, b, a;
        for(j=0; j<h; j++)
        {
                for(i=0,l=0; i<w*d; i+=d, l+=4)
                {
                        r = buffer[j * w * d + i + 0];
                        g = buffer[j * w * d + i + 1];
                        b = buffer[j * w * d + i + 2];
                        a = (r + b + g) / 3;
                        imgbuffer[(h - j - 1) * bytes_per_line + l + 0] = b;
                        imgbuffer[(h - j - 1) * bytes_per_line + l + 1] = g;
                        imgbuffer[(h - j - 1) * bytes_per_line + l + 2] = r;
                        imgbuffer[(h - j - 1) * bytes_per_line + l + 3] = a;
                }
        }
        memset(&bmpHeader, 0, sizeof(BMPHEADER));
        memset(&biHeader, 0, sizeof(BMPINFOHEADER));
        bmpHeader.bfType        = 0x4d42;
        bmpHeader.bfSize        = offset + bytes_per_line * h;
        bmpHeader.bfOffBits     = offset;
        biHeader.biSize         = sizeof(BMPINFOHEADER);
        biHeader.biWidth        = w;
        biHeader.biHeight       = h;
        biHeader.biPlanes       = 0;
        biHeader.biBitCount     = 32;
        biHeader.biCompression  = 0;
        biHeader.biSizeImage    = bytes_per_line * h;
        biHeader.biXPelsPerMeter = 3780;
        biHeader.biYPelsPerMeter = 3780;
        delete [] buffer;
        nWidth = w;
        nHeight = h;
        render->Unlock();
        return 0;
}/*}}}*/

int CSurface::ConvertBMP(const unsigned char * imgbuf, int len)
{/*{{{*/

        render->Lock();
        int     width, height;
        // 取位图头
        const unsigned char *    p;
        memcpy(&bmpHeader, imgbuf, sizeof(BMPHEADER));
        p = imgbuf + sizeof(BMPHEADER);
        memcpy(&biHeader, p, sizeof(BMPINFOHEADER));
        p += biHeader.biSize;

        width = biHeader.biWidth;
        height = (biHeader.biHeight > 0) ? biHeader.biHeight : ( - biHeader.biHeight);

        biHeader.biSizeImage = width * height * biHeader.biBitCount / 8;

        if(biHeader.biWidth < this->nWidth)
                this->nWidth = biHeader.biWidth;
        if(height < this->nHeight)
                this->nHeight = height;
        nSrcLeft = 0;
        nSrcTop  = 0;
        imgbuffer = new unsigned char[width * height * 4];

        // 转化为32位位图
        int             nDepth = biHeader.biBitCount;
        int             i, j, l;
        int             bytes_per_line;

        switch(nDepth)
        {
        case 24:
                if(biHeader.biHeight > 0)
                {
                        bytes_per_line = ((width * 3 + 3) / 4) * 4;
                        for(j=0; j<biHeader.biHeight; j++)
                        {
                                for(i=0, l=0; i<width * 4; i+=4, l+=3)
                                {
                                        imgbuffer[j * width * 4 + i + 0] = p[j * bytes_per_line + l + 0];
                                        imgbuffer[j * width * 4 + i + 1] = p[j * bytes_per_line + l + 1];
                                        imgbuffer[j * width * 4 + i + 2] = p[j * bytes_per_line + l + 2];
                                        imgbuffer[j * width * 4 + i + 3] = 0;
                                }
                        }
                }else{
                        bytes_per_line = ((width * 3 + 3) / 4) * 4;
                        for(j=0; j<height; j++)
                        {
                                for(i=0, l=0; i<width * 4; i+=4, l+=3)
                                {
                                        imgbuffer[j * width * 4 + i + 0] = p[(height - j - 1) * bytes_per_line + l + 0];
                                        imgbuffer[j * width * 4 + i + 1] = p[(height - j - 1) * bytes_per_line + l + 1];
                                        imgbuffer[j * width * 4 + i + 2] = p[(height - j - 1) * bytes_per_line + l + 2];
                                        imgbuffer[j * width * 4 + i + 3] = 0;
                                }
                        }
                }
                memset(&bmpHeader, 0, sizeof(BMPHEADER));
                memset(&biHeader, 0, sizeof(BMPINFOHEADER));
                bmpHeader.bfType 	= 0x4d42;               // "BM"
                bmpHeader.bfReserved1 	= 0;
                bmpHeader.bfReserved2	= 0;
                bmpHeader.bfSize 	= sizeof(BMPHEADER) + sizeof(BMPINFOHEADER) + width * height * 4;
                bmpHeader.bfOffBits	= sizeof(BMPHEADER) + sizeof(BMPINFOHEADER);
                biHeader.biSize		= sizeof(BMPINFOHEADER);
                biHeader.biWidth	= width;
                biHeader.biHeight	= height;
                biHeader.biPlanes	= 0;
                biHeader.biBitCount	= 32;
                biHeader.biCompression	= 0;
                biHeader.biSizeImage	= width * height * 4;
                break;
        case 32:
                memcpy((void *)imgbuffer, (void *)p, biHeader.biSizeImage);
                break;
        default:
                PRINTF("Bitmap Not Support. depth: %d\n", nDepth);
                throw "Bitmap Not Support!";
                render->Unlock();
                return -1;
                break;
        }
        render->Unlock();
        return 0;
}/*}}}*/

int CSurface::Load(const char * imgpath)
{
    if (imgpath == NULL) {
        this->CreateEmptyBitmap();
        return -1;
    }
    if (imgbuffer != NULL) {
        delete[] imgbuffer;
        imgbuffer = NULL;
    }

    int len;
    FILE* fp = fopen(imgpath, "rb");
    if (!fp) {
        this->CreateEmptyBitmap();
        return -1;
    }
    fseek(fp, 0, SEEK_END);
    len = ftell(fp);
    fseek(fp, 0, SEEK_SET);

    unsigned char * buffer = new unsigned char[len];
    int total = 0;
    while (total < len) {
        int ret = fread(buffer + total, 1, len - total, fp);
        if (ret <= 0)
            break;
        total += len;
    }
    unsigned short type = 0;
    memcpy(&type, buffer, sizeof(type));

    int ret;
    if (type == 0x4d42)
        ret = ConvertBMP(buffer, len);
    else if (png_sig_cmp(buffer, 0, 8) == 0)
        ret = ConvertPNG(buffer, len);
    else
        ret = ConvertJPG(buffer, len);

    delete[] buffer;

    if (ret == -1) {
        this->CreateEmptyBitmap();
    }
    return ret;
}

/*
int     CSurface::Load(FILERES * imgbuf)
{
        if(imgbuf == NULL)
        {
                PRINTF("Null Buffer!\n");
                this->CreateEmptyBitmap();
                return -1;
        }

        if(imgbuffer != NULL)
        {
                delete []imgbuffer;
                imgbuffer = NULL;
        }
        int     len = imgbuf->length;

        unsigned short type = 0;
        memcpy(&type, imgbuf->data, sizeof(unsigned short));

        if(type == 0x4d42)
                return ConvertBMP(imgbuf->data, len);
        else if(png_sig_cmp(imgbuf->data, 0, 8) == 0)
                return ConvertPNG(imgbuf->data, len);
        else
                return ConvertJPG(imgbuf->data, len);
}
*/

int     CSurface::SetColorKey(bool bKey, bool bUseFirstColor, unsigned int uKeyColor)
{
        this->bColorKey = bKey;
        this->bUseFirstColor = bUseFirstColor;
        this->uKeyColor = uKeyColor;
        return 0;
}

int     CSurface::Show(bool bShow)
{
        this->bShow = bShow;
        return 0;
}

int     CSurface::SetAlpha(int Alpha)
{
        if(Alpha < -1)
                Alpha = -1;
        if(Alpha > 256)
                Alpha = 256;
        this->nAlpha = Alpha;
        return 0;
}

int     CSurface::SetSrcPos(int left, int top)
{
        if(biHeader.biWidth - left < nWidth)
                left = biHeader.biWidth - nWidth;
        if(biHeader.biHeight - top < nHeight)
                top = biHeader.biHeight - nHeight;
        if(left < 0)
                left = 0;
        if(top < 0)
                top = 0;

        this->nSrcLeft = left;
        this->nSrcTop = top;
        return 0;
}

int     CSurface::MoveTo(int x, int y)
{
        this->ox = x;
        this->oy = y;
        ResetPos();
        return 0;
}

int     CSurface::SetDrawPos(int dx, int dy)
{
        this->x = dx;
        this->y = dy;
        return 0;
}

int     CSurface::ResetPos(void)
{
        this->x = this->ox;
        this->y = this->oy;
        return 0;
}

int     CSurface::Clear(void)
{
    if (!imgbuffer)
        this->CreateEmptyBitmap();
    memset(imgbuffer, 0, nWidth * nHeight * 4);
    return 0;
}
int     CSurface::Fill(int r, int g, int b)
{
    if (!imgbuffer)
        this->CreateEmptyBitmap();
    int i;
    for (i=0; i<nWidth * nHeight * 4; i += 4) {
        imgbuffer[i + 0] = b;
        imgbuffer[i + 1] = g;
        imgbuffer[i + 2] = r;
        imgbuffer[i + 3] = 0;
    }
    return 0;
}

int     CSurface::CreateEmptyBitmap(void)
{
        if(imgbuffer != NULL)
                delete [] imgbuffer;
        this->imgbuffer = new unsigned char [nWidth * nHeight * 4];
        Clear();

        memset(&bmpHeader, 0, sizeof(BMPHEADER));
	memset(&biHeader, 0, sizeof(BMPINFOHEADER));
	bmpHeader.bfType 	= 0x4d42;               // "BM"
	bmpHeader.bfReserved1 	= 0;
	bmpHeader.bfReserved2	= 0;
	bmpHeader.bfSize 	= sizeof(BMPHEADER) + sizeof(BMPINFOHEADER) + nWidth * nHeight * 4;
	bmpHeader.bfOffBits	= sizeof(BMPHEADER) + sizeof(BMPINFOHEADER);
	biHeader.biSize		= sizeof(BMPINFOHEADER);
	biHeader.biWidth	= nWidth;
	biHeader.biHeight	= nHeight;
	biHeader.biPlanes	= 0;
	biHeader.biBitCount	= 32;
	biHeader.biCompression	= 0;
	biHeader.biSizeImage	= nWidth * nHeight * 4;
        
        return 0;
}

int     CSurface::Blt(CSurface * surface)
{
        unsigned int    tcolor = 0, color;
        int             x0, y0;
        int             left, top, right, bottom;
        unsigned char * p0;
        unsigned char * p1;

        if(!surface->bShow)
        {
                return 0;
        }

        // 先求相交矩形
        if(this->x + this->nWidth < surface->x 
           ||   this->x > surface->x + surface->nWidth
           ||   this->y + this->nHeight < surface->y
           ||   this->y > surface->y + surface->nHeight
        )
        {
                return 0;
        }

        left = std::max<int>(this->x, surface->x);
        right = std::min<int>(this->x + this->nWidth, surface->x + surface->nWidth);
        top = std::max<int>(this->y, surface->y);
        bottom = std::min<int>(this->y + this->nHeight, surface->y + surface->nHeight);

        if(surface->bColorKey)
        {
                if(surface->bUseFirstColor)
                        tcolor = *(unsigned int *)surface->imgbuffer & 0x00ffffff;
                else
                        tcolor = surface->uKeyColor;
                // CTools::Get()->Debug("transparent color is %#x\n", tcolor);
        }

        // 出于效率原因， 透明色与半透明的四种组合分开来做。
        if(surface->nAlpha > 0 && surface->nAlpha < 256)        // 半透明
        {
                if(surface->bColorKey)                          // 最慢的情况， 同时存在ColorKey和透明色。。。
                {
                        for(y0 = 0; y0 < bottom - top; y0 ++)
                        {
                                for(x0 = 0; x0 < right - left; x0 ++)
                                {
                                        p0 = this->imgbuffer + (this->biHeader.biHeight - 1 - (y0 + top - this->y + this->nSrcTop)) * this->biHeader.biWidth * 4 + (x0 + left - this->x + this->nSrcLeft) * 4;
                                        p1 = surface->imgbuffer + (surface->biHeader.biHeight - 1 - (y0 + top - surface->y + surface->nSrcTop)) * surface->biHeader.biWidth * 4 + (x0 + left - surface->x + surface->nSrcLeft) * 4;
                                        color = *(unsigned int *)p1 & 0x00FFFFFF;
                                        if(color == tcolor)
                                        {
                                                continue;
                                        }
                                        p0[0] = p0[0] + (p1[0] - p0[0]) * surface->nAlpha / 256;
                                        p0[1] = p0[1] + (p1[1] - p0[1]) * surface->nAlpha / 256;
                                        p0[2] = p0[2] + (p1[2] - p0[2]) * surface->nAlpha / 256;
                                }
                        }
                }
                else                                            // 可以考虑用MMX来做。
                {
                        for(y0 = 0; y0 < bottom - top; y0 ++)
                        {
                                for(x0 = 0; x0 < right - left; x0 ++)
                                {
                                        p0 = this->imgbuffer + (this->biHeader.biHeight - 1 - (y0 + top - this->y + this->nSrcTop)) * this->biHeader.biWidth * 4 + (x0 + left - this->x + this->nSrcLeft) * 4;
                                        p1 = surface->imgbuffer + (surface->biHeader.biHeight - 1 - (y0 + top - surface->y + surface->nSrcTop)) * surface->biHeader.biWidth * 4 + (x0 + left - surface->x + surface->nSrcLeft) * 4;
                                        p0[0] = p0[0] + (p1[0] - p0[0]) * surface->nAlpha / 256;
                                        p0[1] = p0[1] + (p1[1] - p0[1]) * surface->nAlpha / 256;
                                        p0[2] = p0[2] + (p1[2] - p0[2]) * surface->nAlpha / 256;
                                }
                        }
                }
        }
        else                                                    // 不半透明
        {
                if(surface->nAlpha == 0)                        // 显示背景
                {
                        return 0;
                }
                else                                            // 显示前景
                {
                        if(surface->bColorKey)                  // 存在透明色
                        {
                                for(y0 = 0; y0 < bottom - top; y0 ++)
                                {
                                        for(x0 = 0; x0 < right - left; x0 ++)
                                        {
                                                p0 = this->imgbuffer + (this->biHeader.biHeight - 1 - (y0 + top - this->y + this->nSrcTop)) * this->biHeader.biWidth * 4 + (x0 + left - this->x + this->nSrcLeft) * 4;
                                                p1 = surface->imgbuffer + (surface->biHeader.biHeight - 1 - (y0 + top - surface->y + surface->nSrcTop)) * surface->biHeader.biWidth * 4 + (x0 + left - surface->x + surface->nSrcLeft) * 4;
                                                color = *(unsigned int *)p1 & 0x00FFFFFF;
                                                if(color == tcolor)
                                                {
                                                        continue;
                                                }
                                                p0[0] = p1[0];
                                                p0[1] = p1[1];
                                                p0[2] = p1[2];
                                        }
                                }

                        }
                        else                                    // 简单位图复制 
                        {
                                for(y0 = 0; y0 < bottom - top; y0 ++)
                                {
                                        p0 = this->imgbuffer + (this->biHeader.biHeight - 1 - (y0 + top - this->y + this->nSrcTop)) * this->biHeader.biWidth * 4 + (left - this->x + this->nSrcLeft) * 4;
                                        p1 = surface->imgbuffer + (surface->biHeader.biHeight - 1 - (y0 + top - surface->y + surface->nSrcTop)) * surface->biHeader.biWidth * 4 + (left - surface->x + surface->nSrcLeft) * 4;
                                        memcpy(p0, p1, (right-left) * 4);
                                }
                        }
                }
        }

        return 0;
}

int     CSurface::Blt(CSurface * surface, int x, int y)
{
        int     ox, oy;
        ox = this->x;
        oy = this->y;
        this->MoveTo(x, y);
        this->Blt(surface);
        this->MoveTo(ox, oy);
        return 0;
}




