#ifndef MAINFRAME_H
#define MAINFRAME_H

#include <QWidget>
#include <QTimer>
#include "mainmenu.h"


#define USE_SLIDERMENU
#undef  USE_SLIDERMENU

#define USE_POPMENU

class CMediaForm;
class CBackCamForm;
class CGPSForm;
class CGPRSForm;

class CMainFrame : public QWidget
{
        Q_OBJECT
public:
        explicit CMainFrame(QWidget *parent = 0);

signals:

public slots:
        void OnMenuClick();
        void OnMenuTimer();

        void OnFunc(int func);

private:
        void paintEvent(QPaintEvent *);
        void mouseDoubleClickEvent(QMouseEvent * event);
        void timerEvent(QTimerEvent * event);

        CMainMenu       *       menu;
        int                     folder;
        QTimer          *       menuTimer;


        CGPSForm        *       gpsForm;
        CBackCamForm    *       backForm;
        CMediaForm      *       mediaForm;
        CGPRSForm       *       gprsForm;

        void CreateMenuForm();


public:
        enum
        {
                FUNC_GPS,
                FUNC_MEDIA,
                FUNC_BACKCAM,
                FUNC_GPRS,
        };


};

#endif // MAINFRAME_H
