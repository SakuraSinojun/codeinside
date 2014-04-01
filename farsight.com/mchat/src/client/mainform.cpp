
#include "../mcprotocal.h"
#include "mainform.h"
#include "ui_mainform.h"
#include "network.h"
#include "config.h"
#include "chatform.h"

#include <QTime>
#include <QDate>

#include <QtGui>
#include "loginwin.h"


CMainForm::CMainForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CMainForm)
{
        ui->setupUi(this);

        this->setFixedSize(240, 473);

        pNet = CNetwork::Get();
        pConfig = CConfig::Get();

        connect(pNet->sock, SIGNAL(connected()), this, SLOT(OnConnected()));
        connect(pNet->sock, SIGNAL(disconnected()), this, SLOT(OnDisconnected()));
        connect(pNet->sock, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(OnError()));
        connect(pNet->sock, SIGNAL(readyRead()), this, SLOT(OnReadyRead()));

        this->ui->btnCover->setGeometry(this->rect());
        this->ui->btnCover->setText(QString(tr("正在登陆...")));

        this->adddlg.show();
        this->adddlg.Clear();
        this->adddlg.hide();

        connect(this, SIGNAL(AddOnlineUser(USERINFO)), &adddlg, SLOT(OnAddUser(USERINFO)));

        pNet->Connect();

}

CMainForm::~CMainForm()
{
        delete ui;
}

void CMainForm::OnConnected()
{
        pNet->Login(QString(tr(pConfig->GetUserInfo().username)), QString(tr(pConfig->GetUserInfo().password)));

}

void CMainForm::OnDisconnected()
{
        QMessageBox::critical(this, QString(tr("断开连接")),
                              QString(tr("远程主机已断开连接。")),
                              QMessageBox::Ok);
        this->close();

}

void CMainForm::OnError()
{
         QMessageBox::critical(this, QString(tr("错误")),
                              QString(tr("网络错误 %1 : %2")).arg(pNet->sock->error()).arg(pNet->sock->errorString()),
                              QMessageBox::Ok);
        this->close();
 }

void CMainForm::OnReadyRead()
{
        MCPACK          pack;
        unsigned int      uid;
        std::vector<CHATFORM>::iterator         iter;
        std::vector<unsigned int>::iterator           ituid;
        CHATFORM        cform;
        FILE    *       fp;
        char              path[1024];
        char              buffer[1024];

        memset(&pack, 0, sizeof(pack));

        while(pNet->sock->bytesAvailable() >= sizeof(pack))
        {
                pNet->sock->read((char *)&pack, sizeof(pack));

                switch(pack.action)
                {
                case MA_LOGIN_ANSWER:
                        if(pack.answer.state == 0)
                        {
                                //success
                                pConfig->SetUid(pack.answer.uid);
                                pConfig->SetLogin(true);
                                this->ui->btnCover->hide();
                                pNet->GetUserinfo(pConfig->GetUid());
                                pNet->GetFriendList();
                                timer_heart = this->startTimer(1000);
                                this->timer_NewMsg = this->startTimer(500);


                        }else{
                                QMessageBox::critical(this, QString(tr("登陆")),
                                                      QString(tr("登陆失败. \n 服务器信息:%1")).arg(pack.answer.message),
                                                      QMessageBox::Ok);
                                this->close();
                        }
                        break;
                case MA_ADD_FRIEND:
                        if( QMessageBox::information(this, QString(tr("添加")),
                                                 QString(tr("%1 想添加你为好友，是否同意？")).arg(pack.request.uid),
                                                 QMessageBox::Yes | QMessageBox::No) == QMessageBox::Yes)
                        {
                                uid = pack.request.uid;

                                pack.action = MA_ADD_FRIEND_ANSWER;
                                pack.answer.state = 0;
                                pack.answer.uid = uid;
                                strcpy(pack.answer.message, "heya");
                                pNet->sock->write((const char *)&pack, sizeof(pack));
                                pNet->sock->flush();
                        }else{
                                uid = pack.request.uid;
                                pack.action = MA_ADD_FRIEND_ANSWER;
                                pack.answer.state = 1;
                                pack.answer.uid = uid;
                                strcpy(pack.answer.message, "i dont know you.");
                                pNet->sock->write((const char *)&pack, sizeof(pack));
                                pNet->sock->flush();
                        }
                        break;
                case MA_ADD_FRIEND_ANSWER:
                        pNet->GetFriendList();
                        break;
                case MA_FRIEND_LIST:
                        this->ui->listFriend->clear();
                        for(int i=0; i<MAX_FRIENDS_COUNT; i++)
                        {
                                if(pack.list.uid[i] != 0)
                                {
                                       this->ui->listFriend->addItem(new QListWidgetItem(QString::number(pack.list.uid[i])));
                                }
                        }
                        this->timer1 = this->startTimer(100);
                        break;
                case MA_ACK_USER_INFO:
                        if(pack.userinfo.uid == pConfig->GetUid())
                        {
                                if(pConfig->GetUserInfo().username[0] != '\0')
                                {
                                        fp = fopen("localusers.cfg", "rb+");
                                        if(fp == NULL)
                                        {
                                                fp = fopen("localusers.cfg", "wb+");
                                                if(fp == NULL)
                                                {
                                                        return;
                                                }
                                        }
                                        LOGUSERFILE      luf;
                                        long                       pos;
                                        bool                      flag;
                                        fseek(fp, 0, SEEK_SET);
                                        pos = ftell(fp);
                                        flag = false;
                                        while( fread(&luf, sizeof(luf), 1, fp) == 1 )
                                        {
                                                if(luf.uid == pack.userinfo.uid)
                                                {
                                                        flag = true;
                                                        break;
                                                }
                                                pos = ftell(fp);
                                        }
                                        if(!flag)
                                        {
                                                fseek(fp, 0, SEEK_END);
                                        }else{
                                                fseek(fp, pos, SEEK_SET);
                                        }
                                        strncpy(luf.username,  pConfig->GetUserInfo().username, 32);
                                        strncpy(luf.password, pConfig->GetUserInfo().password, 32);
                                        strncpy(luf.nickname, pack.userinfo.nickname, 32);
                                        luf.iconindex = pack.userinfo.head_pic;
                                        luf.uid = pConfig->GetUid();

                                        fwrite(&luf, sizeof(luf), 1, fp);
                                        fclose(fp);
                                }

                                memcpy(&(pConfig->UserInfo()), &pack.userinfo, sizeof(pack.userinfo));
                                this->ui->lblNick->setText(pack.userinfo.nickname);
                                this->ui->btnHead->setIconSize(QSize(40, 40));


                                this->ui->btnHead->setIcon(QIcon(pConfig->GetIconPath(pack.userinfo.head_pic)));
                        }
                        for(int i=0; i<ui->listFriend->count(); i++)
                        {
                                if((unsigned int)(atoi(ui->listFriend->item(i)->text().toStdString().c_str())) == pack.userinfo.uid)
                                {

                                        ui->listFriend->item(i)->setText(QString(tr("%1\t%2")).arg(pack.userinfo.uid).arg(QString(tr(pack.userinfo.nickname))));
                                        ui->listFriend->item(i)->setIcon(QIcon(pConfig->GetIconPath(pack.userinfo.head_pic)));

                                        // gray when not online.
                                        if(pack.userinfo.onlinestate == 0)
                                        {
                                                ui->listFriend->item(i)->setIcon(QIcon(pConfig->GetIconPath(pack.userinfo.head_pic, true)));
                                        }else{
                                                ui->listFriend->item(i)->setIcon(QIcon(pConfig->GetIconPath(pack.userinfo.head_pic)));
                                        }
                                       break;
                                }
                        }
                        pConfig->AddUserInfo(pack.userinfo);
                        break;
                case MA_ONLINE_USER:
                        //this->adddlg.AddUser(pack.userinfo);
                        emit this->AddOnlineUser(pack.userinfo);
                        break;
                case MA_CHAT:

                        sprintf(path, "data/%dto%d.html", pConfig->GetUid(), pack.chat.uid);
                        fp = fopen(path, "rb+");
                        if(fp == NULL)
                        {
                                fp = fopen(path, "wb+");
                                if(fp == NULL)
                                {
                                        QMessageBox::critical(this, QString(tr("err open")), QString(tr("error when open file %1")).arg(tr(path)), QMessageBox::Ok);
                                        exit(0);
                                }
                                strcpy(buffer, "<meta http-equiv=\"content-type\" content=\"text/html; charset=utf-8\"/><br />");
                                fwrite(buffer, strlen(buffer), 1, fp);
                        }

                        fseek(fp, 0, SEEK_END);
                        sprintf(buffer, "<label style='color: #FF0000;'>%s(%4d-%2d-%2d %2d:%2d:%2d)</label>"
                                "<br><label style='color:#0000FF; padding-left: 20px; padding-right: 20px'>%s</label><br>\n",
                                pConfig->GetUserInfo(pack.chat.uid).nickname,
                                QDate::currentDate().year(), QDate::currentDate().month(), QDate::currentDate().day(),
                                QTime::currentTime().hour(), QTime::currentTime().minute(), QTime::currentTime().second(),
                                pack.chat.message);
                        fwrite(buffer, strlen(buffer), 1, fp);
                        fclose(fp);
                        fflush(fp);

                       for(iter=this->vct.begin(); iter != vct.end(); iter++)
                        {
                                cform = (CHATFORM)(*iter);
                                if(cform.uid == pack.chat.uid)
                                {
                                        if(cform.form->isVisible())
                                        {
                                                cform.form->Refresh();
                                        }else{
                                                vct.erase(iter);
                                                iter = vct.end();
                                        }
                                        break;
                                }
                        }

                        if(iter == vct.end())
                        {
                                for(ituid=this->vctNewMsg.begin(); ituid != vctNewMsg.end(); ituid++)
                                {
                                        uid = (unsigned int)(*ituid);
                                        if(uid == pack.chat.uid)
                                        {
                                                break;
                                        }
                                }
                                if(ituid == vctNewMsg.end())
                                {
                                        vctNewMsg.push_back(pack.chat.uid);
                                }
                        }

                        break;
                default:
                        // un-implementation;
                        break;
                }
                pNet->sock->flush();
        }
        
}

void CMainForm::timerEvent(QTimerEvent * event)
{
        std::vector<unsigned int>::iterator             iter;
        unsigned int            uid1;
        unsigned int            uid2;
        static int                tick = 0;
        static int                infoindex = 0;


        if(event->timerId() == timer1)
        {
                //for(int i=0; i<ui->listFriend->count(); i++)
                if(infoindex < ui->listFriend->count())
                {
                        pNet->GetUserinfo(atoi(ui->listFriend->item(infoindex)->text().toStdString().c_str()));
                        if(ui->listFriend->item(infoindex)->text().length() <= 6)
                        {
                                this->killTimer(this->timer1);
                                timer1 = this->startTimer(100);
                                return;
                        }
                        infoindex ++;
                }else{
                        this->killTimer(this->timer1);
                        this->timer1 = this->startTimer(1000);
                        infoindex = 0;
                }
        }
        else if(event->timerId() == timer_heart)
        {
                pNet->Heart();
        }
        else if(event->timerId() == this->timer_NewMsg)
        {

                for(int i=0; i<ui->listFriend->count(); i++)
                {
                        uid1 = atoi(ui->listFriend->item(i)->text().toStdString().c_str());

                        for(iter =  this->vctNewMsg.begin(); iter != vctNewMsg.end(); iter++)
                        {
                                uid2 = (unsigned int)(*iter);
                                if(uid1 == uid2)
                                {
                                        if(tick != 0)
                                        {
                                                QPixmap pxm(40, 40);
                                                QPixmap pixmap(pConfig->GetIconPath(pConfig->GetUserInfo(uid1).head_pic));
                                                QPainter painter(&pxm);
                                                painter.setBrush(QBrush(QColor(255, 255, 255)));
                                                painter.setPen(QPen(QColor(255, 255, 255)));
                                                painter.drawRect(0, 0, 40, 40);
                                                painter.drawPixmap(-2, -2, pixmap);
                                                QIcon  icon(pxm);
                                                ui->listFriend->item(i)->setIcon(icon);
                                        }else{
                                                ui->listFriend->item(i)->setIcon(QIcon(pConfig->GetIconPath(pConfig->GetUserInfo(uid1).head_pic)));
                                        }
                                        tick = 1 - tick;
                                }
                        }
                }
        }

}

void CMainForm::on_btnAddFriend_clicked()
{
        this->adddlg.show();
        adddlg.Clear();
        pNet->GetOnlineUser();

}

void CMainForm::closeEvent(QCloseEvent *)
{
        exit(0);
}

void CMainForm::on_listFriend_itemDoubleClicked(QListWidgetItem* item)
{
        std::vector<CHATFORM>::iterator         iter;
        std::vector<unsigned int>::iterator           ituid;
        unsigned int            uid;
        CHATFORM          chat;

        uid = atoi(item->text().toStdString().c_str());

        item->setIcon(QIcon(pConfig->GetIconPath(pConfig->GetUserInfo(uid).head_pic)));
        for(ituid=this->vctNewMsg.begin(); ituid != vctNewMsg.end(); ituid++)
        {
                if((unsigned int)(*ituid) == uid)
                {
                        vctNewMsg.erase(ituid);
                        break;
                }
        }

        for(iter=vct.begin(); iter != vct.end(); iter++)
        {
                chat = (CHATFORM)(*iter);
                if( chat.uid == uid )
                {
                        if(chat.form->isVisible())
                        {
                                chat.form->show();
                                return;
                        }else{
                                vct.erase(iter);
                                break;
                        }
                }
        }


        char    path[1024];
        char    buffer[1024];
        sprintf(path, "data/%dto%d.html", pConfig->GetUid(), uid);
        FILE *  fp = fopen(path, "rb+");
        if(fp == NULL)
        {
                fp = fopen(path, "wb+");
                if(fp == NULL)
                {
                        QMessageBox::critical(this, QString(tr("错误")), QString(tr("打开聊天记录文件[%1]失败。")).arg(tr(path)), QMessageBox::Ok);
                        exit(0);
                }
                strcpy(buffer, "<meta http-equiv=\"content-type\" content=\"text/html; charset=utf-8\"/><br />");
                fwrite(buffer, strlen(buffer), 1, fp);
        }
        fflush(fp);
        fclose(fp);


        chat.uid = uid;
        chat.form = new CChatForm();
        chat.form->SetUserInfo(pConfig->GetUserInfo(), pConfig->GetUserInfo(uid));
        chat.form->show();
        // chat.form->Refresh();

        vct.push_back(chat);

}

void CMainForm::paintEvent(QPaintEvent *)
{
        QPainter painter(this);
        QImage  image(QString(tr("data/mainform.PNG")));
        painter.drawImage(0, 0, image);

        /*
        painter.setBrush(QBrush(QColor(182, 226, 243)));
        painter.setPen(QPen(QColor(182, 226, 243)));

        painter.drawRect(this->rect());
        */

}














