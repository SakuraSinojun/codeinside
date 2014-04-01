
#include "backcamform.h"
#include <QPainter>
#include <QMovie>
#include <fstream>
#include "exception.h"
#include <QDebug>

CBackCamForm::CBackCamForm(QWidget *parent) :
    QWidget(parent)
{
        this->setFixedSize(480, 272);
        isCapturing = false;

        this->pixmap = new QPixmap();
        this->capTimer = new QTimer(this);
        this->pressTimer = new QTimer(this);

        connect(pressTimer, SIGNAL(timeout()), this, SLOT(OnPressTimer()));

        try
        {
                this->camera = new CCamera("/dev/video0", 100);
        }
        catch(CGPSException& e)
        {
                qDebug() << e.ErrDesc();
                return;
        }

        isCapturing = true;

        this->capTimer->setSingleShot(false);
        this->capTimer->setInterval(100);
        connect(capTimer, SIGNAL(timeout()), this, SLOT(OnCaptureTimer()));

        this->capTimer->start();

}

CBackCamForm::~CBackCamForm()
{
        delete this->camera;
}


void CBackCamForm::show()
{
        if(this->isCapturing)
        {
                this->camera->Resume();
        }
        QWidget::show();
}

void CBackCamForm::hide()
{
        if(this->isCapturing)
        {
                this->camera->Pause();
        }

        QWidget::hide();
}


void CBackCamForm::OnCaptureTimer()
{
        int     filesize = camera->GetImageFileSize();

        std::ofstream   file("/tmp/gpscamtmp");
        file.write((const char *)camera->GetBuffers(), filesize);
        file.close();
        pixmap->load(QString("/tmp/gpscamtmp"));
        this->repaint();
}

void CBackCamForm::paintEvent(QPaintEvent *)
{
        QPainter        painter(this);

        if(this->isCapturing)
        {
                //painter.drawPixmap(0, 0, 480, 272, *pixmap);
                painter.drawPixmap(0, 0, *pixmap);
        }
        else
        {
                painter.setBrush(QBrush(QColor(255, 0, 255)));
                painter.setPen(QPen(QColor(255, 0, 255)));
                painter.drawRect(this->rect());
        }

}


void CBackCamForm::mousePressEvent(QMouseEvent *)
{
        this->longPress = false;
        this->pressTimer->setSingleShot(true);
        this->pressTimer->setInterval(1000);
        this->pressTimer->start();
}

void CBackCamForm::OnPressTimer()
{
        this->longPress = true;
        this->pressTimer->stop();

        emit this->OnBackToMenu();
}

void CBackCamForm::mouseReleaseEvent(QMouseEvent *)
{
        if(this->longPress)
        {
                return;
        }
        this->pressTimer->stop();
}

