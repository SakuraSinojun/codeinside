
#ifndef VIDEOLISTWIDGET_H
#define VIDEOLISTWIDGET_H

#include <QListWidget>
#include <QTimer>

class CVideoListWidget : public QListWidget
{
    Q_OBJECT
public:
    explicit CVideoListWidget(QWidget *parent = 0);

signals:
        void OnLongPress();

public slots:
        void OnPressTimer();

private:
        void mousePressEvent(QMouseEvent *event);
        void mouseReleaseEvent(QMouseEvent *e);


        QTimer  *       pressTimer;
        bool            longPress;

};

#endif // VIDEOLISTWIDGET_H
