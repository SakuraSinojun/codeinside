
#include "mainmenu.h"
#include "mainframe.h"
#include <QPainter>
#include <QPixmap>
#include <QMouseEvent>

CMainMenu::CMainMenu(QWidget *parent) :
    QWidget(parent)
{
        QPixmap pix1(QString("images/menu1.png"));
        int     cx;
        int     cy;
        cx = 480 / 4 -pix1.width();
        cy = 272 - pix1.height();
        cx /= 2;
        cy /= 2;

        rect1.setRect(cx, cy, pix1.width(), pix1.height());
        rect2.setRect(480/4+cx, cy, pix1.width(), pix1.height());
        rect3.setRect(480/4*2 + cx, cy, pix1.width(), pix1.height());
        rect4.setRect(480/4*3+cx, cy, pix1.width(), pix1.height());
}


void CMainMenu::paintEvent(QPaintEvent *)
{
        QPainter        painter(this);


        painter.setPen(QPen(QColor(0, 255, 0)));
        painter.setBrush(QBrush(QColor(0, 255, 0)));
        painter.drawRect(this->rect());

        QPixmap pix(QString("images/menubg.png"));
        QPixmap pix1(QString("images/menu1.png"));
        QPixmap pix2(QString("images/menu2.png"));
        QPixmap pix3(QString("images/menu3.png"));
        QPixmap pix4(QString("images/menu4.png"));

        painter.drawPixmap(rect(), pix);
        painter.drawPixmap(rect1, pix1);
        painter.drawPixmap(rect2, pix2);
        painter.drawPixmap(rect3, pix3);
        painter.drawPixmap(rect4, pix4);

}

void CMainMenu::mousePressEvent(QMouseEvent *event)
{

        QPoint  pos = event->pos();

        if(this->PtInRect(pos, rect1))
        {
                emit this->OnButton(CMainFrame::FUNC_GPS);
        }
        else if(this->PtInRect(pos, rect2))
        {
                emit this->OnButton(CMainFrame::FUNC_BACKCAM);
        }
        else if(this->PtInRect(pos, rect3))
        {
                emit this->OnButton(CMainFrame::FUNC_MEDIA);
        }
        else if(this->PtInRect(pos, rect4))
        {
                emit this->OnButton(CMainFrame::FUNC_GPRS);
        }

        //emit this->OnClick();
}

bool CMainMenu::PtInRect(QPoint pt, QRect rt)
{
        if(pt.x() < rt.left() || pt.x() > rt.right() || pt.y() < rt.top() || pt.y() > rt.bottom())
        {
                return false;
        }

        return true;
}




















