

#include "videolistwidget.h"

CVideoListWidget::CVideoListWidget(QWidget *parent) :
    QListWidget(parent)
{
        this->pressTimer = new QTimer(this);
        this->pressTimer->setSingleShot(true);

        connect(pressTimer, SIGNAL(timeout()), this, SLOT(OnPressTimer()));

}

void CVideoListWidget::OnPressTimer()
{
        this->longPress = true;
        emit this->OnLongPress();
}

void CVideoListWidget::mousePressEvent(QMouseEvent *event)
{

        this->longPress = false;
        this->pressTimer->start(1000);

        QListWidget::mousePressEvent(event);
}

void CVideoListWidget::mouseReleaseEvent(QMouseEvent *e)
{
        if(longPress)
        {
                return;
        }
        this->pressTimer->stop();

        QListWidget::mouseReleaseEvent(e);
}









