
#include "mediaform.h"
#include <QApplication>
#include <QEventLoop>
#include <QTimer>
#include <stdlib.h>
#include <QFileInfo>
#include <QDir>
#include <QFile>
#include <QFileInfoList>
#include <QStringList>
#include <QString>
#include <QTime>
#include <QDebug>


CMediaForm::CMediaForm(QWidget *parent) :
    QWidget(parent)
{


        //this->list = new QListWidget(this);
        this->list = new CVideoListWidget(this);
        this->list->clear();
        this->list->show();
        this->fileVector.empty();
        connect(this->list, SIGNAL(itemClicked(QListWidgetItem*)), this, SLOT(OnItemClicked(QListWidgetItem*)));
        connect(this->list, SIGNAL(OnLongPress()), this, SIGNAL(OnBackToMenu()));

#ifdef USE_PHONON
        mediaObject = new Phonon::MediaObject(this);
        videoWidget = new Phonon::VideoWidget(this);
        audioOutput = new Phonon::AudioOutput(Phonon::VideoCategory, this);

        Phonon::createPath(mediaObject, videoWidget);
        Phonon::createPath(mediaObject, audioOutput);

        connect(mediaObject, SIGNAL(stateChanged(Phonon::State,Phonon::State)),
                this, SLOT(stateChanged(Phonon::State)));
#endif

#ifdef USE_QPROCESS
        proc = new QProcess(this);
        //proc->setProcessChannelMode(QProcess::SeparateChannels);
        proc->setProcessChannelMode(QProcess::MergedChannels);
#endif

        this->watchTimer = new QTimer(this);
        watchTimer->setSingleShot(false);
        connect(watchTimer, SIGNAL(timeout()), this, SLOT(watchMediaFile()));

        watchTimer->setInterval(1000);

        this->pressTimer = new QTimer(this);
        pressTimer->setSingleShot(true);
        connect(pressTimer, SIGNAL(timeout()), this, SLOT(OnPressTimer()));


        this->list->setFixedSize(480, 272);
        watchMediaFile();
        this->list->show();

}
#ifdef USE_PHONON
void CMediaForm::stateChanged(Phonon::State newState)
{
        switch (newState)
        {
        case Phonon::ErrorState:
                qDebug("%s", mediaObject->errorString().toStdString().c_str());
                mediaObject->stop();
                watchMediaFile();
                list->show();

                break;
        case Phonon::PlayingState:
                videoWidget->setAspectRatio(Phonon::VideoWidget::AspectRatioWidget);
                videoWidget->setAspectRatio(Phonon::VideoWidget::AspectRatioAuto);
                break;
        default:
                break;
        }

}
#endif


void CMediaForm::hide()
{
        QWidget::hide();
}

void CMediaForm::show()
{
        watchMediaFile();
        QWidget::show();
}

void CMediaForm::Pause()
{
#ifdef USE_PHONON
        mediaObject->pause();
#endif

}

void CMediaForm::Play()
{
#ifdef USE_PHONON
        mediaObject->play();
#endif


}

void CMediaForm::Stop()
{
#ifdef USE_PHONON
        mediaObject->stop();
#endif
#ifdef USE_QPROCESS
        proc->write("q\n");
        proc->waitForFinished();
        //proc->kill();
#endif

}

void CMediaForm::Play(QString filename)
{

#ifdef USE_PHONON
        mediaObject->stop();
        mediaObject->setCurrentSource(item->text());
        mediaObject->play();
#endif
#ifdef USE_QPROCESS
        //proc->kill();
        QString command = "./mplayer -vf scale=480:272 " + filename + " -wid " + QString::number(this->winId());
        qDebug() << filename;
        qDebug() << command;
        proc->start(command);
#endif

}

void CMediaForm::SwitchPlayAndPause()
{

#ifdef USE_PHONON
        if(mediaObject->state() == Phonon::PlayingState)
        {
                Pause();
        }
        else
        {
                Play();
        }
#endif

#ifdef USE_QPROCESS
//        proc->write("p");
        proc->putChar('p');
#endif

}



void CMediaForm::resizeEvent(QResizeEvent *)
{

#ifdef USE_PHONON
        videoWidget->resize(this->size());
#endif

}

void CMediaForm::mousePressEvent(QMouseEvent *)
{
        longPress = false;

        this->pressTimer->start(1000);

}

void CMediaForm::mouseReleaseEvent(QMouseEvent *)
{
        if(longPress)
        {
                return;
        }
        else
        {
                this->pressTimer->stop();
                this->SwitchPlayAndPause();
        }
}

void CMediaForm::OnPressTimer()
{
        longPress = true;
        Stop();
        watchMediaFile();
        this->list->show();

}


void CMediaForm::watchMediaFile()
{

        qDebug() << __FUNCTION__;

        QString         file;

        char    *       strSearchPath;
        strSearchPath = getenv("GG_MEDIAPATH");

        if(strSearchPath == NULL)
        {
                //strSearchPath = (char *)QApplication::applicationFilePath().toStdString().c_str();
                strSearchPath = (char *)".";
                //strSearchPath = (char *)".";
        }

        std::vector<QString>::iterator  itor;

        fileVector.clear();

        findFile(QString(strSearchPath));

        list->clear();
        QPixmap pix("images/movie.png");
        list->setIconSize(pix.size());

        for(itor=fileVector.begin(); itor!=fileVector.end(); itor++)
        {
                list->addItem(new QListWidgetItem(QIcon(pix), *itor));
        }

}

void CMediaForm::findFile(QString path)
{     
        QDir            dir(path);

        if(!dir.exists())
        {
                return;
        }

        dir.setFilter(QDir::Dirs | QDir::Files);
        dir.setSorting(QDir::DirsLast);
        QFileInfoList fList = dir.entryInfoList();

        int i = 0;
        for(i=0; i<fList.size(); i++)
        {
                QFileInfo fileinfo = fList.at(i);
                if(fileinfo.fileName()=="." || fileinfo.fileName()=="..")
                {
                        continue;
                }

                bool bisDir = fileinfo.isDir();
                if(bisDir)
                {
                        this->findFile(fileinfo.filePath());
                }
                else
                {
                        if(fileinfo.suffix() == "avi" ||
                           fileinfo.suffix() == "mp4")
                        {

                                qDebug("%s",fileinfo.absoluteFilePath().toStdString().c_str());                                

                                std::vector<QString>::iterator  iter;
                                for(iter=fileVector.begin(); iter!=fileVector.end(); iter++)
                                {
                                        if(fileinfo.absoluteFilePath() == *iter)
                                        {
                                                continue;
                                        }
                                }
                                fileVector.push_back(fileinfo.absoluteFilePath());
                        }
                }

        }

}

void CMediaForm::OnItemClicked(QListWidgetItem * item)
{

        this->Play(item->text());
        list->hide();

}






















