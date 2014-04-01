#ifndef MEDIAFORM_H
#define MEDIAFORM_H


#define USE_PHONON
#undef USE_PHONON
#define USE_QPROCESS

#include <QWidget>

#include <QTimer>
#include <QString>
#include <vector>
#include <QListWidget>
#include <QTime>
#include <QListWidgetItem>
#include <QProcess>
#include "videolistwidget.h"



#ifdef USE_PHONON
#include <phonon/MediaObject>
#include <phonon/AudioOutput>
#include <phonon/VideoWidget>
#endif



class CMediaForm : public QWidget
{
    Q_OBJECT
public:
    explicit CMediaForm(QWidget *parent = 0);

signals:
        void OnBackToMenu();

public slots:

#ifdef USE_PHONON
        void stateChanged(Phonon::State);
#endif
        void watchMediaFile();
        void OnPressTimer();
        void OnItemClicked(QListWidgetItem*);

public:
        void hide();
        void show();

private:

        void mousePressEvent(QMouseEvent *);
        void mouseReleaseEvent(QMouseEvent *);
        void resizeEvent(QResizeEvent *);

        void findFile(QString path);

#ifdef USE_PHONON
        Phonon::MediaObject *   mediaObject;
        Phonon::VideoWidget *   videoWidget;
        Phonon::AudioOutput *   audioOutput;
#endif
#ifdef USE_QPROCESS
        QProcess        *       proc;
#endif

        QTimer  *               pressTimer;
        QTimer  *               watchTimer;
        QString *               currentFile;
        //QListWidget *           list;
        CVideoListWidget *      list;


        std::vector<QString>    fileVector;

        bool    longPress;

        void Pause();
        void Stop();
        void Play();
        void Play(QString filename);
        void SwitchPlayAndPause();


};

#endif // MEDIAFORM_H
