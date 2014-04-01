
#include "config.h"
#include <stdlib.h>

#include <QObject>
#include <QFile>
#include <QDataStream>

CConfig * CConfig::m_handler = NULL;

CConfig::CConfig()
{
        this->GetServerPort();
        this->GetServerIP();
        this->m_login = false;

}

CConfig::~CConfig()
{
}

CConfig * CConfig::Get()
{
        if(m_handler == NULL)
        {
                m_handler = new CConfig();
        }
        return m_handler;
}

void CConfig::SetServer(QString ip, int port)
{
        this->m_ip = ip;
        this->m_port = port;

        QFile   file("ipaddress.bin");

        if(file.open(QFile::ReadWrite))
        {
                QDataStream out(&file);
                out<<ip<<port;
                file.close();
        }
}

QString CConfig::GetServerIP()
{
        QFile   file("ipaddress.bin");
        if(file.open(QFile::ReadWrite))
        {
                QDataStream in(&file);
                in>>m_ip>>m_port;
                file.close();
        }else{
                m_ip = QString(QObject::tr("127.0.0.1"));
                m_port = 10234;
        }
        return m_ip;
}

int CConfig::GetServerPort()
{
        QFile   file("ipaddress.bin");
        if(file.open(QFile::ReadWrite))
        {
                QDataStream in(&file);
                in>>m_ip>>m_port;
                file.close();
        }else{
                m_ip = QString(QObject::tr("127.0.0.1"));
                m_port = 10234;
        }

        return m_port;
}

bool CConfig::IsLogin()
{
        return this->m_login;
}

void CConfig::SetLogin(bool l)
{
        this->m_login = l;
}

void CConfig::SetUid(unsigned int uid)
{
        this->m_uid = uid;
}

unsigned int CConfig::GetUid()
{
        return this->m_uid;
}

void CConfig::SetUserInfo(USERINFO &info)
{
        this->m_uinfo = info;
}

USERINFO CConfig::GetUserInfo()
{
        return this->m_uinfo;
}

USERINFO& CConfig::UserInfo()
{
        return this->m_uinfo;
}

void CConfig::SetTempUser(USERINFO &info)
{
        this->m_tmpuser = info;
}

USERINFO& CConfig::TempUser()
{
        return m_tmpuser;
}

USERINFO CConfig::GetUserInfo(unsigned int uid)
{
        std::vector<USERINFO>::iterator         iter;
        USERINFO        uinfo;

        for(iter=vct.begin(); iter != vct.end(); iter++)
        {
                uinfo = (USERINFO)(*iter);
                if(uinfo.uid == uid)
                {
                        return uinfo;
                }
        }

        memset(&uinfo, 0, sizeof(uinfo));
        return uinfo;
}

void CConfig::AddUserInfo(USERINFO info)
{
        std::vector<USERINFO>::iterator         iter;
        USERINFO        uinfo;

        for(iter=vct.begin(); iter != vct.end(); iter++)
        {
                uinfo = (USERINFO)(*iter);
                if(uinfo.uid == info.uid)
                {
                        vct.erase(iter);
                        vct.push_back(info);
                        return;
                }
        }

        vct.push_back(info);

}

QString CConfig::GetIconPath(int index, bool bGray)
{
        QString str;

        if(!bGray)
        {
                str = QString(QObject::tr("data/%1.png")).arg(index);
        }else{
                str = QString(QObject::tr("data/%1g.png")).arg(index);
        }
        return str;

}















