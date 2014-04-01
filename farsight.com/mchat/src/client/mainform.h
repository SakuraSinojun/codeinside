#ifndef MAINFORM_H
#define MAINFORM_H

#include <QWidget>
#include <QDialog>

#include "addfrienddlg.h"

#include <vector>

namespace Ui {
    class CMainForm;
}

class CNetwork;
class CConfig;
class CChatForm;

class CMainForm : public QWidget//QDialog
{
        Q_OBJECT

public:
        explicit CMainForm(QWidget *parent = 0);
        ~CMainForm();

private:
        Ui::CMainForm *ui;

        CNetwork *              pNet;
        CConfig *                 pConfig;
        int                           timer1;
        int                           timer_heart;
        CAddFriendDlg        adddlg;

        typedef struct
        {
                unsigned int                            uid;
                CChatForm       *                    form;
        }CHATFORM, *PCHATFORM;

        std::vector<CHATFORM>     vct;
        std::vector<unsigned int>       vctNewMsg;
        int                                       timer_NewMsg;

protected:
        void timerEvent(QTimerEvent *);
        void closeEvent(QCloseEvent *);
        void paintEvent(QPaintEvent *);

signals:
        void AddOnlineUser(USERINFO info);

private slots:
        void on_listFriend_itemDoubleClicked(QListWidgetItem* item);
        void on_btnAddFriend_clicked();
        void OnConnected();
        void OnDisconnected();
        void OnError();
        void OnReadyRead();



};

#endif // MAINFORM_H
