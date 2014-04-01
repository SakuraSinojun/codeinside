
#ifndef GPRSFORM_H
#define GPRSFORM_H

#include <QWidget>
#include <QTimer>
#include <QString>
#include <QRect>
#include <QPixmap>
#include <QProcess>

class CGPRSForm : public QWidget
{
        Q_OBJECT
public:
        explicit CGPRSForm(QWidget *parent = 0);
        ~CGPRSForm();
signals:
        void OnBackToMenu();

public slots:
        void OnPressTimer();
        void OnGPSTimer();
        void OnGPRSErr();
        void OnGPRSOut();
        void OnGPRSRead();

private:
        void paintEvent(QPaintEvent *);
        void mousePressEvent(QMouseEvent *);
        void mouseReleaseEvent(QMouseEvent *);
        void closeEvent(QCloseEvent *);

        QTimer  *       pressTimer;
        QTimer  *       gpsTimer;


        QString         phno;

        bool            IsLocked;
        bool            sent;

        bool PtInRect(QPoint pt, QRect rt);

        QRect           rcBtn[10];
        QRect           rcLock;
        QRect           rcClear;
        QRect           rcSend;
        QRect           rcPhone;


        QProcess   *    proc;


        float           lockedlat;
        float           lockedlon;
        float           currentlat;
        float           currentlon;

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

#endif // GPRSFORM_H
