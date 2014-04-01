#ifndef LOGINWIN_H
#define LOGINWIN_H

#include <QWidget>
#include <QDialog>

class QLabel;
class CLogComboBox;
class QLineEdit;
class QCheckBox;
class QPushButton;

typedef struct LOGUSERFILE
{
        unsigned int    uid;
        char               username[32];
        char               password[32];

        int                 iconindex;
        char               nickname[32];
}LOGUSERFILE;

class CLoginWin : public QDialog //QWidget
{
        Q_OBJECT
public:
        explicit CLoginWin(QDialog *parent = 0);

signals:

public slots:
        void OnRegister(QString str);


        void OnOption();
        void OnLogin();

        void OnConnected();
        void OnDisconnected();
        void OnError();
        void OnReadyRead();



protected:
        void paintEvent(QPaintEvent *);
        void closeEvent(QCloseEvent *);

        void ReadLocalUsers();

private:
        CLogComboBox    *       cbUsername;
        QLineEdit           *       txtPassword;
        QLabel               *       lblRegister;
        QCheckBox         *       cbRemember;
        QCheckBox         *       cbAutoLogin;
        QPushButton       *       btnOption;
        QPushButton       *       btnLogin;

};

#endif // LOGINWIN_H

