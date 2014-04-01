


#include "gps.h"

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <termios.h>
#include <errno.h>
#include <string.h>
#include <iostream>
using namespace std;


CGPS::CGPS(const char * com, int bps, int bits, int stopbits, int parity)
{
        cout << "before open " << com << endl;
        // open
        fd = open(com, O_RDWR);
        printf("fd=%d\n", fd);

        if(fd < 0)
        {
                perror("open");
                throw __LINE__;
        }

        cout << "setting bps" << endl;
        // set bps;
        struct termios  opt;
        tcgetattr(fd, &opt);
        tcflush(fd, TCIOFLUSH);
        cfsetispeed(&opt, bps);
        cfsetospeed(&opt, bps);
        if(tcsetattr(fd, TCSANOW, &opt) != 0)
        {
                perror("tcsetattr");
                throw   __LINE__;
        }
        tcflush(fd, TCIOFLUSH);


        cout << "setting parity" << endl;
        // set parity
        tcgetattr(fd, &opt);
        opt.c_cflag &= ~CSIZE;
        opt.c_cflag |= CS8;

        switch(parity)
        {
        case 'j':
        case 'J':
                opt.c_cflag &= INPCK;  // enable;
                opt.c_cflag |= PARODD;  // J check                
                opt.c_cflag |= INPCK;  // disable;
                break;
        case 'o':
        case 'O':
                opt.c_cflag &= INPCK;
                opt.c_cflag &= ~PARODD;
                opt.c_cflag |= INPCK;
                break;
        default:
                printf("No such parity.\n");
                throw __LINE__;
                break;
        }

        cout << "setting stopbits" << endl;
        switch(stopbits)
        {
        case 1:
                opt.c_cflag &= ~CSTOPB;
                break;
        case 2:
                opt.c_cflag |= ~CSTOPB;
                break;
        default:
                printf("No such stopbits.\n");
                throw __LINE__;
                break;
        }

        tcflush(fd, TCIOFLUSH);
        opt.c_cc[VTIME] = 0;
        opt.c_cc[VMIN] = 0;

        if(tcsetattr(fd, TCSANOW, &opt) != 0)
        {
                perror("tcsetattr");
                throw __LINE__; 
        }

        cout << "leave" << __func__ << endl;
        
}

CGPS::~CGPS()
{
        close (fd);
}

GPSINFO& CGPS::GetLocation()
{
        int     count;
        char    buffer[1024];
        char *  gpsbuf;

        memset(buffer, 0, sizeof(buffer));
        cout << "read com file fd=" << fd << endl;
        count = read(fd, buffer, sizeof(buffer));
        if(count < 0)
        {
                perror("read");
                throw __LINE__; 
        }

        cout << "read " << count << "bytes." << endl;

        if(count == 0)
        {
                return this->gpsinfo;
        }

        gpsbuf = strstr(buffer, "$GPRMC");
        if(gpsbuf == NULL)
        {
                //perror("strstr");
                //throw __LINE__;
                return this->gpsinfo;
        }
        for(unsigned int i=0; i<strlen(gpsbuf); i++)
        {
                if(gpsbuf[i] == '\n')
                {
                        gpsbuf[i] = '\0';
                }
        }

        sscanf(gpsbuf, "$GPRMC,%[^,],%c,%f,%c,%f,%c,%f,%f,%[^,]",
                gpsinfo.utc_time, &(gpsinfo.status), 
               &(gpsinfo.latitude_value), &(gpsinfo.latitude),
              &(gpsinfo.longtitude_value), &(gpsinfo.longtitude),
             &(gpsinfo.speed), &(gpsinfo.azimuth_angle), gpsinfo.utc_data);

        return this->gpsinfo;

}




