
#ifdef __TEST

#if defined(WIN32) || defined(_WIN32)
#include <windows.h>
#endif
#include "type.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "jpeglib.h"

int main()
{
        struct jpeg_decompress_struct   cinfo;
        struct jpeg_error_mgr           jerr;
        JSAMPROW                        row_pointer;
        int                             w, h, d;
        int                             bytes_per_line;
        int                             offset;
        unsigned char *                 buffer;
        unsigned char *                 newBuffer;
        unsigned char *                 bf;
        int                             len;
        int                             bytes_read;
        int                             ret;

        FILE *  fp = fopen("test.jpg", "rb");
        if(fp == NULL)
        {
                perror("fopen1");
                return -1;
        }

        fseek(fp, 0, SEEK_END);
        len = ftell(fp);
        fseek(fp, 0, SEEK_SET);
        bf = (unsigned char *)malloc(len);
        ret = 0;
        while(ret < len)
        {
                bytes_read = fread(bf + ret, 1, len - ret, fp);
                if(bytes_read <= 0)
                        break;
                ret += bytes_read;
        }

        cinfo.err = jpeg_std_error(&jerr);
        jpeg_create_decompress(&cinfo);
        // fseek(fp, 0, SEEK_SET);
        jpeg_stdio_src(&cinfo, (FILE *)bf);
        jpeg_set_buffer_len(len);
        jpeg_read_header(&cinfo, 1);

        w = cinfo.image_width;
        h = cinfo.image_height;
        d = cinfo.num_components;

        bytes_per_line = w * 4;
        while((bytes_per_line & 3) != 0)
        {
                bytes_per_line ++;
        }
        
        offset = sizeof(BMPHEADER) + sizeof(BMPINFOHEADER);
        newBuffer = (unsigned char *)malloc(offset + bytes_per_line * h);
        buffer = (unsigned char *)malloc(w * d * h);
        memset(buffer, 0, w * d * h);
        
        jpeg_start_decompress(&cinfo);
        while(cinfo.output_scanline < cinfo.output_height)
        {
                row_pointer = &buffer[cinfo.output_scanline * w * d];
                jpeg_read_scanlines(&cinfo, &row_pointer, 1);
        }
        fclose(fp);

        int     i, j, l;
        int     r, g, b, a;
        for(j=0; j<h; j++)
        {
                for(i=0,l=0; i<w * d; i+=d, l+=4)
                {
                        r = buffer[j * w * d + i + 0];
                        g = buffer[j * w * d + i + 1];
                        b = buffer[j * w * d + i + 2];
                        a = (r + b + g) / 3;
                        newBuffer[offset + j * bytes_per_line + l + 0] = b;
                        newBuffer[offset + j * bytes_per_line + l + 1] = g;
                        newBuffer[offset + j * bytes_per_line + l + 2] = r;
                        newBuffer[offset + j * bytes_per_line + l + 3] = a;
                }
        }
        BMPHEADER       bmpHeader;
        BMPINFOHEADER   biHeader;
        memset(&bmpHeader, 0, sizeof(BMPHEADER));
        memset(&biHeader, 0, sizeof(BMPINFOHEADER));
        bmpHeader.bfType        = 0x4d42;
        bmpHeader.bfSize        = offset + bytes_per_line * h;
        bmpHeader.bfOffBits     = offset;
        biHeader.biSize         = sizeof(BMPINFOHEADER);
        biHeader.biWidth        = w;
        biHeader.biHeight       = h;
        biHeader.biPlanes       = 1;
        biHeader.biBitCount     = 32;
        biHeader.biCompression  = 0;
        biHeader.biSizeImage    = bytes_per_line * h;
        biHeader.biXPelsPerMeter = 3780;
        biHeader.biYPelsPerMeter = 3780;
        
        memcpy(newBuffer, &bmpHeader, sizeof(BMPHEADER));
        memcpy(newBuffer + sizeof(BMPHEADER), &biHeader, sizeof(BMPINFOHEADER));
        free(buffer);

        fp = fopen("output.bmp", "wb");
        if(fp == NULL)
        {
                perror("fopen2");
                return -1;
        }

        ret = fwrite(newBuffer, 1, offset + bytes_per_line * h, fp);
        if(ret != offset + bytes_per_line * h)
        {
                perror("fwrite");
        }
        fclose(fp);
        free(newBuffer);
        return 0;
}

#if defined(WIN32) || defined(_WIN32)
int WINAPI WinMain(
        HINSTANCE hInstance,
        HINSTANCE hPrevInstance,
        LPSTR lpCmdLine,
        int nCmdShow
        )
{
        return main();
}
#endif

#endif


