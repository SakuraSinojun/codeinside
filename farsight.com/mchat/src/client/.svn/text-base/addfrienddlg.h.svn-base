#ifndef ADDFRIENDDLG_H
#define ADDFRIENDDLG_H

#include "../mcprotocal.h"

#include <QDialog>
#include <QListWidgetItem>


namespace Ui {
    class CAddFriendDlg;
}

class CAddFriendDlg : public QDialog
{
    Q_OBJECT

public:
    explicit CAddFriendDlg(QWidget *parent = 0);
    ~CAddFriendDlg();

        void AddUser(USERINFO info);
        void Clear();

public slots:
        void OnAddUser(USERINFO info);


protected:
        void closeEvent(QCloseEvent *);
        void paintEvent(QPaintEvent *);

public:
    Ui::CAddFriendDlg *ui;

private slots:
    void on_listWidget_itemDoubleClicked(QListWidgetItem* item);
};

#endif // ADDFRIENDDLG_H
