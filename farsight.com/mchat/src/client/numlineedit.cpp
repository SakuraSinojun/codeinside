

#include"numlineedit.h"

#include <QKeyEvent>
#include <QRegExp>
#include <QRegExpValidator>

CNumLineEdit::CNumLineEdit(QWidget *parent) :
    QLineEdit(parent)
{
       QRegExp reg("^[1-9][0-9]{0,4}$");

       this->setValidator(new QRegExpValidator(reg, this->parent()));
}

/*
void CNumLineEdit::keyPressEvent(QKeyEvent * event)
{
        event->accept();
        return ;

        if(event->key() < '0' || event->key() > '9')
        {
                event->ignore();
        }else{
                event->accept();
        }
}
*/

