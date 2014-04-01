
#include <QApplication>
#include <QCoreApplication>
#include "mainframe.h"
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <errno.h>
#include <sys/stat.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdio.h>
#include <stdlib.h>

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
        umask(0);
#endif

        QCoreApplication::setApplicationName(QString("gpsglobal"));
        QApplication    app(argc, argv);


        //qDebug("%s", app.applicationFilePath().toStdString().c_str());
        //return 0;

        CMainFrame *    frame = new CMainFrame(0);

        frame->show();

        app.exec();
        delete frame;

        return 0;
}


