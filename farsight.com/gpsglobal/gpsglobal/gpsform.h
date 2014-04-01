#ifndef GPSFORM_H
#define GPSFORM_H

#include <QWidget>
#include <QTimer>
#include <QRect>
#include <QPixmap>
#include <QPoint>
#include <QPushButton>
#include <QRect>

#include "sw.h"

class CGPSForm : public QWidget
{
    Q_OBJECT
public:
    explicit CGPSForm(QWidget *parent = 0);

signals:
        void OnBackToMenu();

public slots:
        void OnPressTimer();
        void OnGPSTimer();
        void OnBtnTimer();
        void OnFlashTimer();

public:
        void    show();
        void    hide();

private:
        void    paintEvent(QPaintEvent *);
        void    mousePressEvent(QMouseEvent *);
        void    mouseReleaseEvent(QMouseEvent *);


        bool PtInRect(QPoint pt, QRect rt);


        QRect           rcUp;
        QRect           rcDown;
        QRect           rcLeft;
        QRect           rcRight;
        QRect           rcZoomOut;
        QRect           rcZoomIn;
        QRect           rcReset;

        QPixmap *       pixUp;
        QPixmap *       pixDown;
        QPixmap *       pixLeft;
        QPixmap *       pixRight;
        QPixmap *       pixZoomOut;
        QPixmap *       pixZoomIn;
        QPixmap *       pixReset;

        QTimer *        flashTimer;
        QTimer *        gpsTimer;
        QTimer *        btnTimer;
        QTimer *        pressTimer;
        bool            longPress;
        bool            IsFlash;

        enum
        {
                MODE_NORMAL,
                MODE_CONTROL,
                MODE_DEBUG,
        }mode;

        enum
        {
                BTN_NONE = 0,
                BTN_UP,
                BTN_DOWN,
                BTN_LEFT,
                BTN_RIGHT,
                BTN_ZOOMOUT,
                BTN_ZOOMIN
        }btn;


private:
        float           level;
        QRect           maprc;
        QPixmap *       pixmap;

        int             centerx;
        int             centery;

        int             currentx;
        int             currenty;


        CSW     *       sw;

        QPoint Gps2Pix(float lon, float lat);
        void   AdjustMap(int x, int y);
        QPoint  PixM2S(int x, int y);
        QPoint  PixS2M(int x, int y);
        QPointF PixS2Gps(int x, int y);
        QPointF PixM2Gps(int x, int y);

        std::vector<QPoint>   result;

private:
        typedef struct GPSINFO
        {
                char    utc_time[64];
                char    status;
                float   latitude_value;
                char    latitude;
                float   longtitude_value;
                char    longtitude;
                float   speed;
                float   azimuth_angle;
                char    utc_data[64];
        }GPSINFO;

        int     shmid;

        GPSINFO *       pInfo;

};

#endif // GPSFORM_H
