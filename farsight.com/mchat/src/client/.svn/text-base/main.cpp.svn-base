
#include <QApplication>
#include <QMessageBox>
#include <QObject>
#include <QString>
#include <QTextCodec>

#include "loginwin.h"
#include "mainform.h"
#include "config.h"


int main(int argc, char * argv[])
{
        QApplication    app(argc, argv);
        QTextCodec::setCodecForCStrings(QTextCodec::codecForName("utf8"));
        QTextCodec::setCodecForTr(QTextCodec::codecForName("utf8"));
        QTextCodec::setCodecForLocale(QTextCodec::codecForName("utf8"));

        CLoginWin   *   loginwin = new CLoginWin();
        loginwin->exec();

        CConfig * pConfig = CConfig::Get();

        if(pConfig->IsLogin())
        {
                CMainForm * mainform = new CMainForm();
                mainform->show();
                return app.exec();
        }
        return 0;

        return app.exec();

}

        /*
        delete loginwin;

        CConfig * pConfig = CConfig::Get();
        if(pConfig->IsLogin())
        {

        }

        CMainForm * mainform = new CMainForm();
        mainform->show();
        app.exec();

        return 0;
}
*/

