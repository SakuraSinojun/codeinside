
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>
#include <fcntl.h>
#include <termios.h>
#include <assert.h>
#include <sys/select.h>


int     fd;

void init()
{
        const char * devfile = getenv("GG_GPRSPORT");
        if(devfile == NULL)
        {
                devfile = "/dev/ttyS0";
        }


        struct termios  opts; 

        //cerr << "open " << devfile << endl;
        printf("%s\n", devfile);
        fd = open(devfile,  O_RDWR | O_NOCTTY);// | O_NOCTTY | O_NDELAY); 
        if(fd < 0)
        {
                perror("open"); 
                fflush(stderr);
                return ;
        }


        tcgetattr(fd,  &opts); 
        tcflush(fd, TCIOFLUSH);
        cfmakeraw(&opts); 
        cfsetispeed(&opts,  B115200); 
        cfsetospeed(&opts,  B115200); 
        tcflush(fd,  TCIFLUSH); 
        /*
        opts.c_cflag &= ~CSIZE; 
        opts.c_cflag |= CS8; 
        opts.c_iflag |= IGNPAR; 
        opts.c_cflag &= ~CSTOPB; 
        opts.c_cflag &= ~CRTSCTS; 
        */
        tcsetattr(fd,  TCSANOW,  &opts); 
        //tcflush(fd, TCIOFLUSH);


}

int Write(const char * msg, int len)
{
        int     ret;
        int     bytes;
        bytes = 0;
        while(true)
        {
                ret = write(fd, msg+bytes, len-bytes);
                if(ret < 0)
                {
                        perror("write");
                        return -1;
                }
                if(ret == 0)
                {
                        break;
                }
                bytes += ret;
        }
        return bytes;
}

int Read(char * out, int len)
{
        int     ret;
        int     bytes = 0;
        
        ret = read(fd, out+bytes, len-bytes);
        if(ret < 0)
        {
                perror("read");
                return -1;
        }
        return ret;

}

void dump(const char * data, int len)
{
        int     i;

        printf("len=%d\n", len);
        printf("HEX DATA:\n");
        for(i=0; i<len; i++)
        {
                printf("%2x ", data[i] & 0xff);
        }
        printf("\n");
        printf("ASCII DATA:\n");
        printf("%s\n", data);
}


void QuerySignal()
{
        char            buffer[1024];
        int             ret;
        pid_t           pid;

        pid = fork();
        if(pid < 0)
        {
                perror("fork");
                return ;
        }
        if(pid == 0)
        {
                strcpy(buffer, "AT+CSQ\r");
                printf("%s\n", buffer);
                ret = Write(buffer, strlen(buffer));
                return;
        }
        else
        {
                while(true)
                {
                        memset(buffer, 0, 1024);
                        ret = Read(buffer, 1024);
                        if(ret < 0)
                        {
                                return;
                        }
                        dump(buffer, ret);
                }
        }

}

void SendSMS()
{
        char    buffer[1024];
        int     ret;

        strcpy(buffer, "AT+CMGS=25\r");
        printf("%s\n", buffer);
        ret = Write(buffer, strlen(buffer));
        sleep(10);
        ret = Read(buffer, 1024);
        dump(buffer, ret);

        strcpy(buffer, "0011000d91688112601001F60008010a00680065006c006c006f\x1a");
        ret = Write(buffer, strlen(buffer));
        sleep(10);
        ret = Read(buffer, 1024);
        dump(buffer, ret);

}

void selectproc()
{
        fd_set  fdsr;
        int     ret;
        struct timeval tv;
        char    buffer[1024];

        while(true)
        {
                FD_ZERO(&fdsr);
                FD_SET(fd, &fdsr);
                FD_SET(STDIN_FILENO, &fdsr);
                tv.tv_sec = 3;
                tv.tv_usec = 0;
                ret = select(fd+1, &fdsr, NULL, NULL, &tv);
                if(ret < 0)
                {
                        perror("select");
                        return;
                }
                if(ret == 0)
                {
                        //cerr << "timeout" << endl;
                        continue;
                }

                if(FD_ISSET(fd, &fdsr))
                {
                        memset(buffer, 0, 1024);
                        ret = read(fd, buffer, 1024);
                        if(ret <= 0)
                        {
                                perror("read");
                        }
                        printf("%s", buffer);
                }
                if(FD_ISSET(STDIN_FILENO, &fdsr))
                {
                        memset(buffer, 0, 1024);
                        fgets(buffer, 1024, stdin);
                        buffer[strlen(buffer)] = '\r';
                        buffer[strlen(buffer)] = '\0';
                        ret = write(fd, buffer, strlen(buffer)); 
                        if(ret <= 0)
                        {
                                perror("write");
                        }
                }
        }
 
}

int main()
{
        init();
        //QuerySignal();
        selectproc();
        return 0;
}



