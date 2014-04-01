#ifndef CHATFORM_H
#define CHATFORM_H

#include <QWidget>
#include "../mcprotocal.h"


namespace Ui {
    class CChatForm;
}

class CChatForm : public QWidget
{
    Q_OBJECT

public:
        explicit CChatForm(QWidget *parent = 0);
        ~CChatForm();

        void SetUserInfo(USERINFO infosender, USERINFO inforeceiver);

        void Refresh();

protected:
        void timerEvent(QTimerEvent *);
        void paintEvent(QPaintEvent *);

private:
        Ui::CChatForm *ui;

        USERINFO        m_sender;
        USERINFO        m_receiver;
        int                     m_timer1;


private slots:
    void on_webView_loadFinished(bool );
    void on_pushButton_4_clicked();

};

#endif // CHATFORM_H
