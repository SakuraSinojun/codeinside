
#include "gprsform.h"
#include <QtGui>
#include <QFont>
#include <QFontMetrics>
#include <QDebug>
#include <fstream>
#include <string>



#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ipc.h>
#include <sys/shm.h>


static const int BTN_WIDTH = 120;
static const int BTN_HEIGHT = 68;

CGPRSForm::CGPRSForm(QWidget *parent) :
    QWidget(parent)
{

        this->sent = false;

        pressTimer = new QTimer(this);
        connect(pressTimer, SIGNAL(timeout()), this, SLOT(OnPressTimer()));

        int     i, j;
        int     cx;
        int     cy;
        int     w, h;

        cx = 480 / 4 - BTN_WIDTH;
        cy = 272 / 4 - BTN_HEIGHT;
        cx /= 2;
        cy /= 2;
        w = BTN_WIDTH;
        h = BTN_HEIGHT;

        for(i=0; i<3; i++)
        {
                for(j=0; j<3; j++)
                {
                        rcBtn[i * 3 + j + 1].setRect(480 / 4 * j + cx, 272 / 4 *(i+1) + cy, w, h);
                }
        }


        rcLock.setRect(480 / 4 * 3 + cx, 272 / 4 * 3 + cy, w, h);
        rcBtn[0].setRect(480 / 4 * 3 + cx, 272 / 4 * 2 + cy, w, h);
        rcClear.setRect(480 / 4 * 3 + cx, 272 / 4 * 1 + cy, w, h);
        rcSend.setRect(480 / 4 * 3 + cx, cy, w, h);
        rcPhone.setRect(30, 12, 300, 50);


        phno = "";
        IsLocked = false;

        proc = new QProcess(this);
        proc->setProcessChannelMode(QProcess::MergedChannels);
        QString command = "./gprsproc";
        qDebug() << command;
        proc->start(command);


        //connect(proc, SIGNAL(readyReadStandardError()), this, SLOT(OnGPRSErr()));
        //connect(proc, SIGNAL(readyReadStandardOutput()), this, SLOT(OnGPRSOut()));
        connect(proc, SIGNAL(readyRead()), this, SLOT(OnGPRSRead()));


        key_t   key;
        key = ftok("/tmp", 'a');
        shmid = -1;
        pInfo = NULL;
        if(key == -1)
        {
                perror("ftok");
                return;
        }
        shmid = shmget(key, sizeof(GPSINFO), 0666|IPC_CREAT);
        if(shmid == -1)
        {
                perror("shmget");
                return;
        }

        this->pInfo = (GPSINFO*)shmat(shmid, NULL, 0);
        if(pInfo == (GPSINFO*)(-1))
        {
                pInfo = NULL;
                perror("shmat");
                return;
        }

        this->gpsTimer = new QTimer(this);
        gpsTimer->setSingleShot(false);
        gpsTimer->start(1000);

}

CGPRSForm::~CGPRSForm()
{
        qDebug() << "exit";
        proc->write("exit\r\n");
        proc->kill();
}

void CGPRSForm::closeEvent(QCloseEvent *)
{

}


void CGPRSForm::paintEvent(QPaintEvent *)
{
        QPainter        painter(this);

        QPixmap         pixmap;

        painter.setBrush(QColor(0, 0, 0));
        painter.drawRect(this->rect());

        if(IsLocked)
        {
                pixmap.load("images/gprsunlock.png");
        }
        else
        {
                pixmap.load("images/gprslock.png");
        }

        painter.drawPixmap(this->rect(), pixmap);

        painter.setBrush(QColor(255, 255, 255));
        painter.setPen(QColor(255, 255, 255));
        painter.drawRect(rcPhone);

        painter.setPen(QColor(255, 0, 0));
        QFont   ft("Arial", 20);
        ft.setPixelSize(40);
        ft.setLetterSpacing(QFont::AbsoluteSpacing, 2);
        painter.setFont(ft);
        painter.drawText(rcPhone, this->phno);


}

void CGPRSForm::OnPressTimer()
{
        emit    this->OnBackToMenu();
        this->pressTimer->stop();
}

void CGPRSForm::mousePressEvent(QMouseEvent * event)
{
        this->pressTimer->setSingleShot(true);
        this->pressTimer->start(1000);

        int     i;
        for(i=0; i<10; i++)
        {
                if(this->PtInRect(event->pos(), rcBtn[i]))
                {
                        if((int)phno.toStdString().find_first_not_of("0123456789") >= 0)
                        {
                                phno = "";
                        }
                        if(phno.length() > 11)
                        {
                                break;
                        }
                        phno += QString((char)(i + '0'));
                }
        }
        if(PtInRect(event->pos(), rcClear))
        {
                phno.clear();
        }

        std::string     buffer;
        if(PtInRect(event->pos(), rcLock))
        {
                if(phno.length()==0)
                {
                        return;
                }
                if((int)phno.toStdString().find_first_not_of("0123456789") >= 0)
                {
                        return;
                }
                if(IsLocked)
                {
                        std::ifstream   file("lock.pho");
                        file >> buffer;
                        file.close();
                        if(phno == QString(buffer.c_str()))
                        {
                                IsLocked = false;
                                phno = "UNLOCKED";
                        }
                        else
                        {
                                phno = "ERROR";
                        }
                }
                else
                {
                        std::ofstream   file("lock.pho");
                        file << phno.toStdString();
                        file.close();
                        IsLocked = true;
                        phno = "LOCKED";
                        this->lockedlat = pInfo->latitude_value;
                        this->lockedlon = pInfo->longtitude_value;
                }

        }
        if(PtInRect(event->pos(), rcSend))
        {
                char            buff[1024];
                if((int)phno.toStdString().find_first_not_of("0123456789") >= 0)
                {
                        phno = "ERROR";
                }
                else
                {
                        sprintf(buff, "SMS,%s,Your car is now at (%f, %f).\r\n",
                                phno.toStdString().c_str(),
                                pInfo->longtitude_value/100.0f,
                                pInfo->latitude_value/100.0f);
                        proc->write(buff);
                        qDebug() << buff;
                        phno = "SENDING";
                        sent = true;
                }
        }

        this->repaint();
}

void CGPRSForm::mouseReleaseEvent(QMouseEvent *)
{
        this->pressTimer->stop();
}

bool CGPRSForm::PtInRect(QPoint pt, QRect rt)
{
        if(pt.x() < rt.left() || pt.x() > rt.right() || pt.y() < rt.top() || pt.y() > rt.bottom())
        {
                return false;
        }

        return true;
}

void CGPRSForm::OnGPRSErr()
{
}

void CGPRSForm::OnGPSTimer()
{
        char            buffer[1024];

        if(!this->IsLocked)
        {
                return;
        }

        this->currentlat = pInfo->latitude_value;
        this->currentlon = pInfo->longtitude_value;

        sprintf(buffer, "SMS,%s,ALARM..YOUR CAR IS MOVING...\r\n", phno.toStdString().c_str());

        if(currentlat != lockedlat || currentlon != lockedlon)
        {
                proc->write(buffer);

        }

}

void CGPRSForm::OnGPRSOut()
{

}

void CGPRSForm::OnGPRSRead()
{
        char            buffer[1024];
        std::string     line;
        std::string     ph;

        memset(buffer, 0, 1024);
        proc->read(buffer, 1024);
        qDebug() << buffer;

        line = buffer;

        if(sent)
        {
                qDebug() << buffer << endl;
                if(strstr(buffer, "OK") != NULL)
                {
                        phno = "OK";
                        sent = false;
                        this->repaint();
                }
                else if(strstr(buffer, "ERROR") != NULL)
                {
                        phno = "FAIL";
                        sent = false;
                        this->repaint();
                }
        }

        if(strstr(buffer, "SMS,") != NULL)
        {
                line.erase(0, strstr(buffer, "SMS") - buffer);
        }

        if(strncmp(line.c_str(), "SMS,", 4) != 0)
        {
                return;
        }

        line.erase(0, 4);
        ph = line.substr(0, line.find_first_of(','));
        if(ph.length() < 11)
        {
                return;
        }
        ph.erase(0, 2);

        sprintf(buffer, "SMS,%s,Your car is now at (%f, %f).\r\n",
                ph.c_str(), pInfo->longtitude_value/100.0f, pInfo->latitude_value/100.0f);
        //qDebug() << "wirte buffer:" << buffer;

        proc->write(buffer);

}




