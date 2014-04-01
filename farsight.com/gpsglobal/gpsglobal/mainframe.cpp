

#include "mainframe.h"
#include <QPainter>
#include <QMouseEvent>
#include <QApplication>
#include <QTimer>
#include <QMessageBox>

#include "backcamform.h"
#include "gpsform.h"
#include "mediaform.h"
#include "gprsform.h"


const int       MENUBUTTON_SIZE  = 100;
const int       MENUMOVE_STEP    = 20;

CMainFrame::CMainFrame(QWidget *parent) :
                QWidget(parent) //, Qt::FramelessWindowHint)
{
        this->setFixedSize(480, 272);
        this->setWindowFlags(Qt::FramelessWindowHint);

        this->menuTimer = new QTimer(this);
        connect(menuTimer, SIGNAL(timeout()), this, SLOT(OnMenuTimer()));


        this->gpsForm = new CGPSForm(this);
        this->gpsForm->setFixedSize(480, 272);

        this->mediaForm = new CMediaForm(this);
        this->mediaForm->setFixedSize(480, 272);
        this->mediaForm->hide();

        this->backForm = new CBackCamForm(this);
        this->backForm->setFixedSize(480, 272);
        this->backForm->hide();

        this->gprsForm = new CGPRSForm(this);
        this->gprsForm->setFixedSize(480, 272);
        this->gprsForm->hide();


        this->menu = NULL;
        this->CreateMenuForm();


        folder = 0;

#ifdef USE_POPMENU
        connect(gpsForm, SIGNAL(OnBackToMenu()), this, SLOT(OnMenuClick()));
        connect(backForm, SIGNAL(OnBackToMenu()), this, SLOT(OnMenuClick()));
        connect(mediaForm, SIGNAL(OnBackToMenu()), this, SLOT(OnMenuClick()));
        connect(gprsForm, SIGNAL(OnBackToMenu()), this, SLOT(OnMenuClick()));
#endif

}


void CMainFrame::paintEvent(QPaintEvent *)
{

        QPainter        painter(this);

        painter.setBrush(QBrush(QColor(0, 0, 0)));
        painter.setPen(QPen(QColor(0, 0, 0)));
        painter.drawRect(0, 0, 480, 272);

}

void CMainFrame::mouseDoubleClickEvent(QMouseEvent *event)
{
        if(event->button() == Qt::LeftButton)
        {
               qApp->exit(0);
        }

}


void CMainFrame::timerEvent(QTimerEvent *)
{

}

void CMainFrame::OnMenuClick()
{
        if(folder != 0)
        {
                return;
        }

#ifdef USE_SLIDERMENU
        if(this->menu->pos().x() >= 0)
        {
                //qDebug("folder");
                folder = -1;
        }

        if(this->menu->pos().x() <= MENUBUTTON_SIZE - 480)
        {
                //qDebug("Unfolder");
                folder = 1;
        }
#endif

#ifdef USE_POPMENU
        if(this->menu->isVisible())
        {
                folder = -1;
        }else{
                menu->setGeometry(this->rect().width()/2,
                                  0, 0, this->rect().height());
                folder = 1;
        }

        this->menu->show();
#endif

        this->menuTimer->setInterval(10);
        this->menuTimer->setSingleShot(false);
        this->menuTimer->start();
}

void CMainFrame::OnMenuTimer()
{
        QPoint          pos;

        if(folder == 0)
        {
                this->menuTimer->stop();
        }

        pos = this->menu->pos();

#ifdef USE_POPMENU
        if(folder > 0)
        {
                if(pos.x()<=0)
                {
                        this->menu->show();
                        this->menu->setGeometry(this->rect());
                        this->menuTimer->stop();
                        folder = 0;
                        return;
                }
                QRect rc = menu->rect();
                rc.setWidth(rc.width() + MENUMOVE_STEP * 2);
                pos.setX(pos.x() - MENUMOVE_STEP);
                this->menu->setGeometry(pos.x(), this->rect().top(), rc.width(), this->rect().height());

        }
        else
        {
                if(pos.x()>=240)
                {
                        this->menu->hide();
                        this->menu->setGeometry(this->rect());
                        folder = 0;
                        return;
                }
                QRect rc = menu->rect();
                rc.setWidth(rc.width() - MENUMOVE_STEP * 2);
                pos.setX(pos.x() + MENUMOVE_STEP);
                this->menu->setGeometry(pos.x(), this->rect().top(), rc.width(), this->rect().height());
        }

#endif

#ifdef USE_SLIDERMENU
        if(folder > 0)
        {
                if(pos.y() <= 0)
                {
                        this->menu->move(0, 0);
                        this->menuTimer->stop();
                        folder = 0;
                        return;
                }

                pos.setX((int)((float)pos.x() + (float)MENUMOVE_STEP * 480.0 / 272.0));
                pos.setY(pos.y() - MENUMOVE_STEP);
                this->menu->move(pos);
        }
        else
        {
                if(pos.y() >= 272 - MENUBUTTON_SIZE)
                {
                        this->menu->move(MENUBUTTON_SIZE - 480, 272-MENUBUTTON_SIZE);
                        this->menuTimer->stop();
                        folder = 0;
                        return;
                }

                pos.setX((int)((float)pos.x() - (float)MENUMOVE_STEP * 480.0 / 272.0));
                pos.setY(pos.y() + MENUMOVE_STEP);
                this->menu->move(pos);
        }
#endif

}

void CMainFrame::OnFunc(int func)
{
        switch(func)
        {
        case FUNC_MEDIA:
                this->gpsForm->hide();
                this->backForm->hide();
                this->gprsForm->hide();
                this->mediaForm->show();
                break;
        case FUNC_BACKCAM:
                this->gpsForm->hide();
                this->mediaForm->hide();
                this->gprsForm->hide();
                this->backForm->show();
                break;
        case FUNC_GPRS:
                this->gpsForm->hide();
                this->mediaForm->hide();
                this->backForm->hide();
                this->gprsForm->show();
                break;
        default:
                qDebug("error when switch func.");
                // no break here.
        case FUNC_GPS:
                this->backForm->hide();
                this->mediaForm->hide();
                this->gprsForm->hide();
                this->gpsForm->show();
                //this->form = new CGPSForm(this);
                break;
        }

        this->menu->raise();
        this->OnMenuClick();

}

void CMainFrame::CreateMenuForm()
{

        this->menu = new CMainMenu(this);
        this->menu->setGeometry(this->rect());
#ifdef USE_SLIDERMENU
        this->menu->move(MENUBUTTON_SIZE - 480, 272 - MENUBUTTON_SIZE);
#endif
#ifdef USE_POPMENU
        this->menu->move(0, 0);
#endif

        this->menu->show();

        connect(this->menu, SIGNAL(OnClick()), this, SLOT(OnMenuClick()));
        connect(this->menu, SIGNAL(OnButton(int)), this, SLOT(OnFunc(int)));

        menu->raise();

}















