
#ifndef CONFIG_H
#define CONFIG_H

#include "../mcprotocal.h"
#include <QString>
#include <vector>

class CConfig
{
public:
        ~CConfig();
        static CConfig * Get();

        void            SetServer(QString ip, int port);
        QString      GetServerIP();
        int              GetServerPort();

        bool            IsLogin();
        void            SetLogin(bool l);

        void            SetUid(unsigned int uid);
        unsigned int    GetUid();
        void            SetUserInfo(USERINFO& info);

        USERINFO GetUserInfo();
        USERINFO& UserInfo();

        USERINFO GetUserInfo(unsigned int uid);

        void            SetTempUser(USERINFO& info);
        USERINFO&       TempUser();

        void            AddUserInfo(USERINFO info);

        QString         GetIconPath(int index, bool bGray = false);

private:
        CConfig();
        static CConfig * m_handler;

        QString             m_ip;
        int                     m_port;
        bool                   m_login;
        unsigned int        m_uid;
        USERINFO       m_uinfo;
        USERINFO        m_tmpuser;

        std::vector<USERINFO>   vct;

};

#endif // CONFIG_H
