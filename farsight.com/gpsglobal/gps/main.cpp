

#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ipc.h>
#include <sys/shm.h>

#include "gps.h"


#define DEAMON
//#undef  DEAMON

int main(int argc, char * argv[])
{
        pid_t   pid;

#ifdef DEAMON
        pid = fork();
        if(pid < 0)
        {
                perror("fork");
                return -1;
        }
        if(pid > 0)
        {
                exit(0);
        }

        pid = fork();
        if(pid < 0)
        {
                perror("fork2");
                exit(-1);
        }
        if(pid > 0)
        {
                exit(0);
        }

        pid = setsid();
        if(pid == (pid_t)(-1))
        {
                perror("setsid");
                exit(-1);
        }
        if(chdir("/tmp") != 0)
        {
                perror("chdir");
                exit(-1);
        }
        umask(0);
        for(int fd=0; fd<getdtablesize(); fd++)
        {
                close(fd);
        }

#endif

        const char * comfile = getenv("GG_GPSPORT");
        if(comfile == NULL)
        {
                comfile = "/dev/ttyS0";
        }

        printf("open file: %s\n", comfile);

//        CGPS(char * com, int bps, int bits, int stopbits, int parity);
 
        key_t   key;
        key = ftok("/tmp", 'a');
        if(key == -1)
        {
                perror("ftok");
                exit(-1);
        }

        int             shmid;
        GPSINFO *       pInfo;

        shmid = shmget(key, sizeof(GPSINFO), 0666|IPC_CREAT);
        if(shmid == -1)
        {
                perror("shmget");
                exit(-1);
        }

        if((pInfo = (GPSINFO*)shmat(shmid, NULL, 0)) == (GPSINFO*)(-1))
        {
                perror("shmat");
                exit(-1);
        }

        CGPS    * gps;
        GPSINFO        gpsinfo;

        try
        {
                printf("before new\n");
                gps = new CGPS(comfile, 9600, 8, 1, 'j');
                while(true)
                {
                        printf("reading...\n");
                        gpsinfo = gps->GetLocation();
                        memcpy(pInfo, &gpsinfo, sizeof(GPSINFO));

                        printf("%f, %f\n", pInfo->latitude_value, pInfo->longtitude_value);
                        sleep(1);
                }
        }
        catch(int&      e)
        {
                printf("err no: %d\n", e);
        }

        

        return 0;

}


