#ifndef MAINMENU_H
#define MAINMENU_H

#include <QWidget>
#include <QRect>

class CMainMenu : public QWidget
{
    Q_OBJECT
public:
    explicit CMainMenu(QWidget *parent = 0);

signals:

        void OnClick();

        void OnButton(int func);

public slots:



private:
        void paintEvent(QPaintEvent *);
        void mousePressEvent(QMouseEvent *);

        bool PtInRect(QPoint pt, QRect rt);


        QRect           rect1;
        QRect           rect2;
        QRect           rect3;
        QRect           rect4;



};

#endif // MAINMENU_H
