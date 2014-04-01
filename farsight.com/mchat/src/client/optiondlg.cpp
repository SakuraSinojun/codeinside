
#include "optiondlg.h"
#include "numlineedit.h"
#include "config.h"

#include <QtGui>


COptionDlg::COptionDlg(QWidget *parent) :
    QDialog(parent)
{
        this->setFixedSize(320, 141);

        this->cbIpaddress = new QComboBox(this);
        this->txtPort = new CNumLineEdit(this);
        this->btnOK = new QPushButton(QString(tr("OK")), this);
        this->btnCancel = new QPushButton(QString(tr("Cancel")), this);

        this->cbIpaddress->setGeometry(34, 56, 133, 21);
        this->txtPort->setGeometry(180, 56, 90, 21);
        this->btnOK->setGeometry(158, 115, 70, 23);
        this->btnCancel->setGeometry(238, 115, 70, 23);

        this->cbIpaddress->setEditable(true);
        this->cbIpaddress->addItem(CConfig::Get()->GetServerIP());
        this->cbIpaddress->setCurrentIndex(0);
        this->txtPort->setText(QString::number(CConfig::Get()->GetServerPort()));

        connect(btnOK, SIGNAL(clicked()), this, SLOT(OnOK()));
        connect(btnCancel, SIGNAL(clicked()), this, SLOT(OnCancel()));

}

void COptionDlg::paintEvent(QPaintEvent *)
{
        QPainter        painter(this);

        painter.drawImage(0, 0, QImage(QString(tr("data/opbg.PNG"))));

}

void COptionDlg::OnCancel()
{
        this->close();
}

void COptionDlg::OnOK()
{
        CConfig * pConfig = CConfig::Get();

        pConfig->SetServer(this->cbIpaddress->currentText(), this->txtPort->text().toInt());

        this->close();

}














