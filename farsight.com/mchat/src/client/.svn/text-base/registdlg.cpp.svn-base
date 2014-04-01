
#include "registdlg.h"
#include "network.h"
#include "../mcprotocal.h"
#include "config.h"


#include <QtGui>
#include <QPainter>
#include <QRegExp>
#include <QRegExpValidator>


CRegistDlg::CRegistDlg(QWidget *parent) :
    QDialog(parent)
{

        this->setFixedSize(200, 200);

        m_wrapper = new QGridLayout(this);
        m_lblUsername = new QLabel(QString(tr("帐  号")), this);
        m_lblPassword = new QLabel(QString(tr("密  码")), this);
        m_lblNickname = new QLabel(QString(tr("昵  称")), this);
        m_lblAge = new QLabel(QString(tr("年  龄")), this);
        m_lblGender = new QLabel(QString(tr("性  别")), this);
        m_lblHead = new QLabel(QString(tr("头  像")), this);
        m_txtUsername = new QLineEdit(this);
        m_txtPassword = new QLineEdit(this);
        m_txtNickname = new QLineEdit(this);
        m_txtAge = new QLineEdit(this);
        m_cbGender = new QComboBox(this);
        m_cbHead = new QComboBox(this);
        m_btnOK = new QPushButton(QString(tr("注  册")), this);
        m_btnCancel = new QPushButton(QString(tr("取  消")), this);

        m_wrapper->addWidget(m_lblUsername, 0, 0);
        m_wrapper->addWidget(m_txtUsername, 0, 1);
        m_wrapper->addWidget(m_lblPassword, 1, 0);
        m_wrapper->addWidget(m_txtPassword, 1, 1);
        m_wrapper->addWidget(m_lblNickname, 2, 0);
        m_wrapper->addWidget(m_txtNickname, 2, 1);
        m_wrapper->addWidget(m_lblAge, 3, 0);
        m_wrapper->addWidget(m_txtAge, 3, 1);
        m_wrapper->addWidget(m_lblGender, 4, 0);
        m_wrapper->addWidget(m_cbGender, 4, 1);
        m_wrapper->addWidget(m_lblHead, 5, 0);
        m_wrapper->addWidget(m_cbHead, 5, 1);
        m_wrapper->addWidget(m_btnOK, 6, 1);
        m_wrapper->addWidget(m_btnCancel, 6, 0);

        this->setLayout(m_wrapper);

        this->m_cbGender->setIconSize(QSize(40, 40));
        this->m_cbHead->setIconSize(QSize(40, 40));
        this->m_cbGender->addItem(QIcon(CConfig::Get()->GetIconPath(19)), QString(tr("男")));
        this->m_cbGender->addItem(QIcon(CConfig::Get()->GetIconPath(17)), QString(tr("女")));
        this->m_cbGender->addItem(QIcon(CConfig::Get()->GetIconPath(0)), QString(tr("其它")));
        this->m_cbGender->setCurrentIndex(2);

        for(int i=0; i<24; i++)
        {
                this->m_cbHead->addItem(QIcon(CConfig::Get()->GetIconPath(i)), QString(tr("%1")).arg(i), i);
        }


        QRegExp reg("^[1-9][0-9]{0,2}$");
        this->m_txtAge->setValidator(new QRegExpValidator(reg, this));

        QRegExpValidator * rev;
        QRegExp reg2("^[a-z|A-Z][a-z|A-Z|0-9|_]{6,31}$");
        rev = new QRegExpValidator(reg2, this);
        this->m_txtUsername->setValidator(rev);

        connect(m_btnOK, SIGNAL(clicked()), this, SLOT(OnOK()));
        connect(m_btnCancel, SIGNAL(clicked()), this, SLOT(OnCancel()));

        connect(&sock, SIGNAL(connected()), this, SLOT(OnConnected()));
        connect(&sock, SIGNAL(disconnected()), this, SLOT(OnDisconnected()));
        connect(&sock, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(OnError()));
        connect(&sock, SIGNAL(readyRead()), this, SLOT(OnReadyRead()));

}

void CRegistDlg::paintEvent(QPaintEvent *)
{
        QPainter painter(this);

        painter.setBrush(QBrush(QColor(182, 226, 243)));
        painter.setPen(QPen(QColor(182, 226, 243)));

        painter.drawRect(this->rect());

}

void CRegistDlg::OnOK()
{

        if(             this->m_txtUsername->text().isEmpty() || this->m_txtPassword->text().isEmpty()
                ||      this->m_txtNickname->text().isEmpty() || this->m_txtAge->text().isEmpty()
        )
        {
                QMessageBox::warning(this, QString(tr("注册")), QString(tr("请认真添写所有表项！")), QMessageBox::Ok);
                return;
        }

        CConfig * pConfig = CConfig::Get();

        this->sock.close();
        sock.connectToHost(pConfig->GetServerIP(), pConfig->GetServerPort());

}

void CRegistDlg::OnCancel()
{
        this->close();
}

void CRegistDlg::OnConnected()
{
        MCPACK          pack;

        pack.action = MA_REGISTER_REQUEST;
        strncpy(pack.regist.username, m_txtUsername->text().toStdString().c_str(), 32);
        strncpy(pack.regist.password,  m_txtPassword->text().toStdString().c_str(), 32);
        strncpy(pack.regist.nickname, m_txtNickname->text().toStdString().c_str(), 32);
        pack.regist.age = m_txtAge->text().toInt();
        pack.regist.gender = m_cbGender->currentIndex();
        pack.regist.head_pic = m_cbHead->currentIndex();

        sock.write((const char *)&pack, sizeof(pack));

}

void CRegistDlg::OnDisconnected()
{
        //        QMessageBox::information(this, "Disconnected", "Disconnected", QMessageBox::Ok);
}

void CRegistDlg::OnError()
{
        if(sock.error() != 1)
        {
                QMessageBox::information(this, QString(tr("错误")), QString(tr("网络错误：%1")).arg(sock.errorString()), QMessageBox::Ok);
        }

}

void CRegistDlg::OnReadyRead()
{
        MCPACK  pack;
        sock.read((char *)&pack, sizeof(pack));

        if(pack.action == MA_REGISTER_ANSWER)
        {
                switch(pack.answer.state)
                {
                case 0:
                        QMessageBox::information(this, QString(tr("注册")), QString(tr("注册成功！.\n 服务器信息:%1")).arg(pack.answer.message), QMessageBox::Ok);
                        this->close();
                        break;
                default:
                        QMessageBox::critical(this, QString(tr("注册")),
                                              QString(tr("注册失败！.\n 服务器信息:%1")).arg(pack.answer.message),
                                              QMessageBox::Ok);
                        break;
                }
        }

}


















