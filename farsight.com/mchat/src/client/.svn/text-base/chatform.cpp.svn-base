
#include "chatform.h"
#include "ui_chatform.h"

#include "config.h"

#include <QtGui>
#include "network.h"
#include <QWebFrame>
#include <QMessageBox>

CChatForm::CChatForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CChatForm)
{
        ui->setupUi(this);
        this->setFixedSize(492, 493);

        this->ui->toolButton->setIconSize(QSize(40, 40));
        //ui->webView->setLocale(QLocale(QString(tr("zh_CN.utf-8"))));

}

CChatForm::~CChatForm()
{
        delete ui;
}

void CChatForm::SetUserInfo(USERINFO infosender, USERINFO inforeceiver)
{
        this->m_sender = infosender;
        this->m_receiver = inforeceiver;

        this->ui->toolButton->setIcon(QIcon(CConfig::Get()->GetIconPath(m_receiver.head_pic)));
        this->ui->label->setText(QString(tr("%1(%2)")).arg(QString(tr(m_receiver.nickname))).arg(m_receiver.uid));

        this->ui->webView->setUrl(QUrl(QString(("data/%1to%2.html")).arg(m_sender.uid).arg(m_receiver.uid)));
        this->setWindowTitle(QString(tr("正在与 %1 交谈")).arg(m_receiver.nickname));
       // this->m_timer1 = this->startTimer(500);

}

void CChatForm::timerEvent(QTimerEvent *)
{
}

void CChatForm::Refresh()
{
        //this->ui->webView->setUrl(QUrl(QString(tr("data/%1to%2.html")).arg(m_sender.uid).arg(m_receiver.uid)));
        //QMessageBox::information(this, "", ui->webView->url().toString(), QMessageBox::Ok);
        this->setWindowTitle(QString(tr("正在与 %1 交谈")).arg(m_receiver.nickname));
        this->ui->webView->reload();
}

void CChatForm::on_pushButton_4_clicked()
{
        CNetwork * pNet = CNetwork::Get();

        FILE    *       fp;
        char              path[1024];
        char              buffer[1024];

        if(this->ui->plainTextEdit->document()->isEmpty())
        {
                return;
        }
        sprintf(path, "data/%dto%d.html", m_sender.uid, m_receiver.uid);
        fp = fopen(path, "rb+");
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
        fseek(fp, 0, SEEK_END);
        sprintf(buffer, "<label style='color: #D85CD8;'>%s(%4d-%2d-%2d %2d:%2d:%2d)</label>"
                "<br /><label style='color:#0000FF; padding-left: 20px; padding-right:20px'>%s</label><br />\n",
                m_sender.nickname,
                QDate::currentDate().year(), QDate::currentDate().month(), QDate::currentDate().day(),
                QTime::currentTime().hour(), QTime::currentTime().minute(), QTime::currentTime().second(),
                ui->plainTextEdit->document()->toHtml().toStdString().c_str());
        fwrite(buffer, strlen(buffer), 1, fp);
        fclose(fp);
        fflush(fp);

        pNet->Chat(m_receiver.uid, 0, this->ui->plainTextEdit->document()->toHtml());

        this->ui->plainTextEdit->clear();
        this->Refresh();

}

void CChatForm::on_webView_loadFinished(bool )
{
        this->ui->webView->page()->mainFrame()->setScrollBarValue(Qt::Vertical, ui->webView->page()->mainFrame()->scrollBarMaximum(Qt::Vertical));
}

void CChatForm::paintEvent(QPaintEvent *)
{
        QPainter painter(this);

        QImage image(QString(tr("data/chatform.PNG")));
        painter.drawImage(0, 0, image);

        /*
        painter.setBrush(QBrush(QColor(182, 226, 243)));
        painter.setPen(QPen(QColor(182, 226, 243)));
        painter.drawRect(this->rect());
        */

}

















