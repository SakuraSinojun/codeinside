#ifndef REGISTDLG_H
#define REGISTDLG_H

#include <QDialog>

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QComboBox>
#include <QGridLayout>
#include <QPushButton>
#include <QtNetwork/QTcpSocket>


class CRegistDlg : public QDialog
{
    Q_OBJECT
public:
    explicit CRegistDlg(QWidget *parent = 0);

signals:

public slots:

        void OnOK();
        void OnCancel();

        void OnConnected();
        void OnDisconnected();
        void OnError();
        void OnReadyRead();


private:
        QGridLayout     *       m_wrapper;

        QLabel          *       m_lblUsername;
        QLabel          *       m_lblPassword;
        QLabel          *       m_lblNickname;
        QLabel          *       m_lblAge;
        QLabel          *       m_lblGender;
        QLabel          *       m_lblHead;

        QLineEdit       *       m_txtUsername;
        QLineEdit       *       m_txtPassword;
        QLineEdit       *       m_txtNickname;
        QLineEdit       *       m_txtAge;
        QComboBox       *       m_cbGender;
        QComboBox       *       m_cbHead;

        QPushButton     *       m_btnOK;
        QPushButton     *       m_btnCancel;

        QTcpSocket              sock;

protected:
        void paintEvent(QPaintEvent *);


};

#endif // REGISTDLG_H
