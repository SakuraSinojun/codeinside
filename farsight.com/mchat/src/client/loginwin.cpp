
#include "../mcprotocal.h"
#include "loginwin.h"
#include "logcombobox.h"
#include "optiondlg.h"
#include "registdlg.h"
#include "registdlg.h"
#include "network.h"
#include "config.h"
#include "mainform.h"


#include <QPainter>
#include <QtGui>




CLoginWin::CLoginWin(QDialog *parent) :
    QDialog(parent)
{

        QRect   rcScreen;

        this->setFixedSize(332, 217);
        rcScreen = QApplication::desktop()->availableGeometry();
        this->move( (rcScreen.width()-332)/2, (rcScreen.height()-217)/2 );
        this->setWindowFlags(Qt::WindowStaysOnTopHint);

        this->cbUsername = new CLogComboBox(this);
        this->txtPassword = new QLineEdit(this);
        this->lblRegister = new QLabel(this);
        this->cbAutoLogin = new QCheckBox(QString(tr("")), this);
        this->cbRemember = new QCheckBox(QString(tr("")), this);
        this->btnLogin = new QPushButton(QString(tr("登陆")), this);
        this->btnOption = new QPushButton(QString(tr("设置")), this);


        this->lblRegister->setText(QString(tr("<a href=\"http://www.baidu.com/\" style=\"TEXT-DECORATION: none;\">注册新帐号</a>")));

        this->cbUsername->setGeometry(62, 80, 184, 22);
        this->txtPassword->setGeometry(62, 116, 184, 22);
        this->lblRegister->move(251, 83);
        this->cbRemember->move(101, 158);
        this->cbAutoLogin->move(179, 158);
        this->btnOption->setGeometry(5, 190, 70, 23);
        this->btnLogin->setGeometry(256, 190, 70, 23);

        this->btnLogin->setDefault(true);
        this->txtPassword->setEchoMode(QLineEdit::Password);
        this->cbUsername->setIconSize(QSize(40, 40));


        connect(lblRegister, SIGNAL(linkActivated(QString)), this, SLOT(OnRegister(QString)));
        connect(btnLogin, SIGNAL(clicked()), this, SLOT(OnLogin()));
        connect(btnOption, SIGNAL(clicked()), this, SLOT(OnOption()));

        this->ReadLocalUsers();

}


void CLoginWin::paintEvent(QPaintEvent *)
{
        QPainter        painter(this);
        //QRect           rctitle;

        painter.drawImage(0, 0, QImage(QString(tr("data/logbg.PNG"))));

        //painter.drawImage();
}

void CLoginWin::OnRegister(QString str)
{
       CRegistDlg       dlg(this);

       dlg.exec();


}

void CLoginWin::closeEvent(QCloseEvent *)
{
        qApp->quit();
}

void CLoginWin::ReadLocalUsers()
{
        FILE    *                fp;
        LOGUSERFILE      luf;
        QString                 path;

        fp = fopen("localusers.cfg", "rb+");
        if(fp == NULL)
        {
               fp = fopen("localusers.cfg", "wb+");
               if(fp == NULL)
               {
                       return;
               }
        }

        while( fread(&luf, sizeof(luf), 1, fp) == 1 )
        {
                path = CConfig::Get()->GetIconPath(luf.iconindex);//QString("data/%1.gif").arg(luf.iconindex);
                this->cbUsername->addItem(QIcon(path), QString(luf.username), QString(luf.password));
        }

        fclose(fp);

}

void CLoginWin::OnLogin()
{
        CConfig * pConfig = CConfig::Get();

        if(this->cbUsername->currentText().isEmpty() || this->txtPassword->text().isEmpty())
        {
                return;
        }

        pConfig->SetLogin(true);
        USERINFO        info;

        memset(&info, 0, sizeof(info));
        strncpy(info.username, cbUsername->currentText().toStdString().c_str(), 32);
        strncpy(info.password, txtPassword->text().toStdString().c_str(), 32);

        pConfig->SetUserInfo(info);

        this->close();

}

void CLoginWin::OnOption()
{
        COptionDlg      dlg(this);

        dlg.exec();

}

void CLoginWin::OnConnected()
{
        CNetwork * pNet = CNetwork::Get();
        pNet->Login(cbUsername->currentText(), txtPassword->text());
}

void CLoginWin::OnError()
{

}

void CLoginWin::OnDisconnected()
{

}

void CLoginWin::OnReadyRead()
{

}












