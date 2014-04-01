
#include "addfrienddlg.h"
#include "ui_addfrienddlg.h"
#include "network.h"
#include "config.h"

#include <QtGui>

CAddFriendDlg::CAddFriendDlg(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CAddFriendDlg)
{
        ui->setupUi(this);
        this->setWindowTitle(QString(tr("添加好友")));
}

CAddFriendDlg::~CAddFriendDlg()
{
        delete ui;
}

void CAddFriendDlg::AddUser(USERINFO info)
{

}

void CAddFriendDlg::Clear()
{
        ui->listWidget->clear();
}

void CAddFriendDlg::OnAddUser(USERINFO info)
{

        for(int i=0; i<ui->listWidget->count(); i++)
        {
                qDebug()<<ui->listWidget->item(i)->text();
                qDebug()<<atoi(ui->listWidget->item(i)->text().toStdString().c_str());

                if((unsigned int)(atoi(ui->listWidget->item(i)->text().toStdString().c_str())) == info.uid)
                {
                        return;
                }
        }

        QListWidgetItem * item = new QListWidgetItem(QIcon(CConfig::Get()->GetIconPath(info.head_pic)),
                                                     QString(tr("%1\t%2")).arg(info.uid).arg(info.nickname));
        ui->listWidget->addItem(item);
        ui->listWidget->repaint();

}

void CAddFriendDlg::closeEvent(QCloseEvent * event)
{
        event->ignore();
        this->hide();
}

void CAddFriendDlg::on_listWidget_itemDoubleClicked(QListWidgetItem* item)
{
        unsigned int    uid;
        uid = atoi(item->text().toStdString().c_str());
        CNetwork::Get()->AddFriend(uid);
        QMessageBox::information(this, QString(tr("添加好友")), QString(tr("好友已添加，请等待对方回应。")), QMessageBox::Ok);
        this->hide();

}

void CAddFriendDlg::paintEvent(QPaintEvent *)
{
        QPainter painter(this);

        painter.setBrush(QBrush(QColor(182, 226, 243)));
        painter.setPen(QPen(QColor(182, 226, 243)));

        painter.drawRect(this->rect());


}




