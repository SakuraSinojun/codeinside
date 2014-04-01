
#include "gpsform.h"
#include <QPainter>
#include <QMouseEvent>
#include <QDebug>

#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ipc.h>
#include <sys/shm.h>



const int  BTN_SIZE  = 50;
const int  RADIUS  = 10;

CGPSForm::CGPSForm(QWidget *parent) :
    QWidget(parent)
{

        this->mode = MODE_NORMAL;
        this->btn = BTN_NONE;
        this->level = 1;
        this->centerx = 0;
        this->centery = 0;

        this->pressTimer = new QTimer(this);
        connect(pressTimer, SIGNAL(timeout()), this, SLOT(OnPressTimer()));

        this->btnTimer = new QTimer(this);
        connect(btnTimer, SIGNAL(timeout()), this, SLOT(OnBtnTimer()));

        pixmap = new QPixmap(QString("map/map.png"));

        this->gpsTimer = new QTimer(this);
        connect(gpsTimer, SIGNAL(timeout()), this, SLOT(OnGPSTimer()));
        gpsTimer->setSingleShot(false);
        gpsTimer->start(100);

        this->flashTimer = new QTimer(this);
        connect(flashTimer, SIGNAL(timeout()), this, SLOT(OnFlashTimer()));
        flashTimer->setSingleShot(false);
        flashTimer->start(500);

        maprc = QRect(0, 0, 480, 272);


        this->pixDown = new QPixmap(QString("images/btndown.png"));
        this->pixUp = new QPixmap(QString("images/btnup.png"));
        this->pixLeft = new QPixmap(QString("images/btnleft.png"));
        this->pixRight = new QPixmap(QString("images/btnright.png"));
        this->pixZoomOut = new QPixmap(QString("images/btnzoomout.png"));
        this->pixZoomIn = new QPixmap(QString("images/btnzoomin.png"));
        this->pixReset = new QPixmap(QString("images/btnreset.png"));


        this->rcUp = QRect(120 - BTN_SIZE/2, 42, BTN_SIZE, BTN_SIZE);
        this->rcDown = QRect(120-BTN_SIZE/2, 230-BTN_SIZE, BTN_SIZE, BTN_SIZE);
        this->rcLeft = QRect(20, 136-BTN_SIZE/2, BTN_SIZE, BTN_SIZE);
        this->rcRight = QRect(220 - BTN_SIZE, 136-BTN_SIZE/2, BTN_SIZE, BTN_SIZE);
        this->rcReset = QRect(120 - BTN_SIZE /2 , 136 - BTN_SIZE / 2 , BTN_SIZE, BTN_SIZE);
        this->rcZoomIn = QRect(360-BTN_SIZE/2, 272/3-BTN_SIZE/2, BTN_SIZE, BTN_SIZE);
        this->rcZoomOut = QRect(360-BTN_SIZE/2, 272/3*2-BTN_SIZE/2, BTN_SIZE, BTN_SIZE);


        sw = new CSW("map/mapmask.png");

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


}

void CGPSForm::show()
{
        mode = MODE_NORMAL;
        QWidget::show();
}

void CGPSForm::hide()
{

        QWidget::hide();
}

void CGPSForm::OnFlashTimer()
{
        this->IsFlash = ! this->IsFlash;
}

void CGPSForm::paintEvent(QPaintEvent *)
{
        QPainter        painter(this);
        painter.drawPixmap(this->rect(), *pixmap, this->maprc);

        if(this->IsFlash)
        {
                QPoint        pt = PixM2S(currentx, currenty);
                painter.setBrush(QBrush(QColor(255, 0, 0)));
                painter.setPen(QPen(QColor(255, 0, 0)));
                painter.drawEllipse(pt, RADIUS, RADIUS);
        }

        if(!this->result.empty())
        {
                painter.setBrush(QBrush(QColor(255, 0, 255)));
                painter.setPen(QPen(QColor(255, 0, 255)));

                std::vector<QPoint>::iterator   iter;
                int     cx, cy;

                for(iter=result.begin(); iter!=result.end(); iter++)
                {
                        cx = this->PixM2S(iter->x()*GRID_SIZE, iter->y()*GRID_SIZE).x();
                        cy = this->PixM2S(iter->x()*GRID_SIZE, iter->y()*GRID_SIZE).y();

                        painter.drawEllipse(cx, cy, GRID_SIZE*level, GRID_SIZE*level);
                }

        }

        switch(mode)
        {
        case MODE_NORMAL:
                break;
        case MODE_CONTROL:
                painter.drawPixmap(rcLeft, *pixLeft);
                painter.drawPixmap(rcRight, *pixRight);
                painter.drawPixmap(rcUp, *pixUp);
                painter.drawPixmap(rcDown, *pixDown);
                painter.drawPixmap(rcReset, *pixReset);
                painter.drawPixmap(rcZoomOut, *pixZoomOut);
                painter.drawPixmap(rcZoomIn, *pixZoomIn);
                break;
        case MODE_DEBUG:
                break;
        default:
                break;
        }
}


void CGPSForm::mousePressEvent(QMouseEvent * event)
{
        this->longPress = false;

        switch(mode)
        {
        case MODE_CONTROL:
                btn = BTN_NONE;
                if(this->PtInRect(event->pos(), this->rcUp))
                {
                        btn = BTN_UP;
                }
                else if(PtInRect(event->pos(), rcDown))
                {
                        btn = BTN_DOWN;
                }
                else if(PtInRect(event->pos(), rcLeft))
                {
                        btn = BTN_LEFT;
                }
                else if(PtInRect(event->pos(), rcRight))
                {
                        btn = BTN_RIGHT;
                }
                else if(PtInRect(event->pos(), rcZoomOut))
                {
                        btn = BTN_ZOOMOUT;
                }
                else if(PtInRect(event->pos(), rcZoomIn))
                {
                        btn = BTN_ZOOMIN;
                }
                else if(PtInRect(event->pos(), rcReset))
                {
                        btn = BTN_NONE;
                        this->centerx = 0;
                        this->centery = 0;
                        this->repaint();
                        break;
                }
                if(btn != BTN_NONE)
                {
                        this->btnTimer->setSingleShot(false);
                        this->btnTimer->setInterval(50);
                        this->btnTimer->start();
                        break;
                }
                // none break here.
        case MODE_NORMAL:
                this->longPress = false;
                this->pressTimer->setSingleShot(true);
                this->pressTimer->setInterval(1000);
                this->pressTimer->start();
                break;
        case MODE_DEBUG:
                break;
        default:
                break;
        }

}

void CGPSForm::OnGPSTimer()
{

        if(pInfo != NULL)
        {
                currentx = this->Gps2Pix(pInfo->longtitude_value/100.0f, pInfo->latitude_value/100.0f).x();
                currenty = this->Gps2Pix(pInfo->longtitude_value/100.0f, pInfo->latitude_value/100.0f).y();
        }
        else
        {
                currentx = this->Gps2Pix(116.333885, 39.990076).x();
                currenty = this->Gps2Pix(116.333885, 39.990076).y();
        }


        currentx = this->Gps2Pix(116.189277,39.59).x();
        currenty = this->Gps2Pix(116.189277,39.59).y();


        this->AdjustMap(currentx, currenty);
        this->repaint();
}

void CGPSForm::OnBtnTimer()
{
        switch(btn)
        {
        case BTN_NONE:
                break;
        case BTN_UP:
                this->centery -= 5;
                break;
        case BTN_DOWN:
                this->centery += 5;
                break;
        case BTN_LEFT:
                this->centerx -= 5;
                break;
        case BTN_RIGHT:
                this->centerx += 5;
                break;
        case BTN_ZOOMOUT:
                this->level /= 1.1f;
                break;
        case BTN_ZOOMIN:
                this->level *= 1.1f;
                break;
        default:
                break;
        }

      //  this->repaint();

}

void CGPSForm::OnPressTimer()
{
        this->longPress = true;
        this->pressTimer->stop();

        switch(this->mode)
        {
        case MODE_NORMAL:
                this->mode = MODE_CONTROL;
                this->repaint();
                break;
        case MODE_CONTROL:
                emit this->OnBackToMenu();
                break;
        case MODE_DEBUG:
                break;
        default:
                break;
        }

}

void CGPSForm::mouseReleaseEvent(QMouseEvent * event)
{
        QPoint src;
        QPoint dst;

        if(this->longPress)
        {
                return;
        }
        this->pressTimer->stop();

        switch(mode)
        {
        case MODE_NORMAL:
                /*
                src = PixM2Gps(currentx, currenty);
                dst = PixS2Gps(event->pos().x(), event->pos().y());
                */
                src = QPoint(currentx, currenty);
                dst = QPoint(PixS2M(event->x(), event->y()));
                qDebug() << "SW: (" << src.x() << "," << src.y() << ") to ("<<dst.x() << "," << dst.y()<< ")";

                this->result.clear();
                this->result = sw->SearchWay(src, dst);
                qDebug() << "finish.";

                break;
        case MODE_CONTROL:
                if(btn == BTN_NONE)
                {
                        // TODO: Search Way:
                        //PixS2Gps
                        src = QPoint(currentx, currenty);
                        dst = QPoint(PixS2M(event->x(), event->y()));
                        this->result.clear();
                        this->result = sw->SearchWay(src, dst);
                        mode = MODE_NORMAL;
                }
                else
                {
                        btn = BTN_NONE;
                        this->btnTimer->stop();
                        this->repaint();
                }
                break;
        case MODE_DEBUG:
                break;
        default:
                break;
        }


}

QPoint CGPSForm::Gps2Pix(float lon, float lat)
{
        QPoint  tmp;
        tmp.setX((lon - sw->GetMinLon()) / (sw->GetMaxLon() - sw->GetMinLon()) * pixmap->width());
        tmp.setY((sw->GetMaxLat() - lat) / (sw->GetMaxLat() - sw->GetMinLat()) * pixmap->height());
        return tmp;

}

void   CGPSForm::AdjustMap(int x, int y)
{
        this->maprc.setLeft((x+centerx) - 240 / level);
        this->maprc.setTop((y+centery) - 136 / level);
        this->maprc.setWidth(480 / level);
        this->maprc.setHeight(272 / level);
}

QPoint  CGPSForm::PixM2S(int x, int y)
{
        QPoint  pt;

        pt.setX((x-maprc.left()) * level);
        pt.setY((y-maprc.top()) * level);
        return pt;
}

QPoint  CGPSForm::PixS2M(int x, int y)
{
        QPoint  pt;
        pt.setX(x / level + maprc.left());
        pt.setY(y / level + maprc.top());
        return pt;
}

QPointF CGPSForm::PixS2Gps(int x, int y)
{
        QPoint  ptM;
        ptM.setX(maprc.left() + x / level);
        ptM.setY(maprc.top() + y / level);


        QPointF lp;
        lp.setX((float)ptM.x() / (float)pixmap->width() * (float)(sw->GetMaxLon() - sw->GetMinLon()) + sw->GetMinLon());
        lp.setY(sw->GetMaxLat() - (float)ptM.y() / pixmap->width() * (sw->GetMaxLat() - sw->GetMinLat()));

        return lp;
}

QPointF CGPSForm::PixM2Gps(int x, int y)
{
        QPointF lp;
        lp.setX((float)x / (float)pixmap->width() * (float)(sw->GetMaxLon() - sw->GetMinLon()) + sw->GetMinLon());
        lp.setY(sw->GetMaxLat() - (float)y / pixmap->width() * (sw->GetMaxLat() - sw->GetMinLat()));
        return lp;
}


bool CGPSForm::PtInRect(QPoint pt, QRect rt)
{
        if(pt.x() < rt.left() || pt.x() > rt.right() || pt.y() < rt.top() || pt.y() > rt.bottom())
        {
                return false;
        }

        return true;
}














