#ifndef OPTIONDLG_H
#define OPTIONDLG_H

#include <QDialog>

class QComboBox;
class QLineEdit;
class QPushButton;
class CNumLineEdit;

class COptionDlg : public QDialog
{
        Q_OBJECT
public:
        explicit COptionDlg(QWidget *parent = 0);

signals:

public slots:

        void OnOK();
        void OnCancel();


public:

protected:
        void paintEvent(QPaintEvent *);

private:
        QComboBox       *       cbIpaddress;
        CNumLineEdit    *       txtPort;
        QPushButton      *       btnOK;
        QPushButton      *       btnCancel;

};

#endif // OPTIONDLG_H



