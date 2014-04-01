#ifndef BACKCAMFORM_H
#define BACKCAMFORM_H

#include <QWidget>
#include <QThread>
#include <QImage>
#include <QPixmap>
#include "camera.h"
#include <QTimer>


class CBackCamForm : public QWidget
{
        Q_OBJECT
public:
         explicit CBackCamForm(QWidget *parent = 0);
        ~CBackCamForm();

signals:
        void OnBackToMenu();

public slots:
        void OnCaptureTimer();
        void OnPressTimer();

public:
        void hide();
        void show();

private:
        void paintEvent(QPaintEvent *);

        void mousePressEvent(QMouseEvent *);
        void mouseReleaseEvent(QMouseEvent *);

        QTimer *        pressTimer;
        bool            longPress;
        CCamera *       camera;
        QPixmap *       pixmap;
        QTimer  *       capTimer;
        bool            isCapturing;

};

#endif // BACKCAMFORM_H
