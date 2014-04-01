

#include "../mcprotocal.h"
#include "network.h"
#include "config.h"

#include <QAbstractSocket>


CNetwork * CNetwork::m_handler = NULL;


CNetwork::CNetwork(QObject *parent) :
    QObject(parent)
{
        sock = new QTcpSocket();

}

CNetwork * CNetwork::Get()
{
        if(m_handler == NULL)
        {
                m_handler = new CNetwork();
        }
        return m_handler;
}

void CNetwork::Login(QString username, QString password)
{
        MCPACK  pack;

        memset(&pack, 0, sizeof(pack));
        pack.action = MA_LOGIN_REQUEST;
        strncpy(pack.regist.username, username.toStdString().c_str(), 32);
        strncpy(pack.regist.password, password.toStdString().c_str(), 32);


        sock->write((const char *)&pack, sizeof(pack));
        sock->flush();

}

void CNetwork::GetUserinfo(unsigned int uid)
{
        MCPACK          pack;
        memset(&pack, 0, sizeof(pack));
        pack.action = MA_QUERY_USER_INFO;
        pack.request.uid = uid;

        sock->write((const char *)&pack, sizeof(pack));
        sock->flush();

}

void CNetwork::GetFriendList()
{
        MCPACK          pack;
        memset(&pack, 0, sizeof(pack));
        pack.action = MA_QUERY_FRIEND_LIST;

        sock->write((const char *)&pack, sizeof(pack));
        sock->flush();

}

void CNetwork::Chat(unsigned int uid, unsigned int gid, QString message)
{
        MCPACK          pack;
        memset(&pack, 0, sizeof(pack));
        pack.action = MA_CHAT;
        pack.chat.uid = uid;
        pack.chat.gid = gid;
        strncpy(pack.chat.message, message.toStdString().c_str(), 1024);

        sock->write((const char *)&pack, sizeof(pack));
        sock->flush();

}

void CNetwork::AddFriend(unsigned int uid)
{
        MCPACK          pack;
        memset(&pack, 0, sizeof(pack));
        pack.action = MA_ADD_FRIEND;
        pack.request.uid = uid;

        sock->write((const char *)&pack, sizeof(pack));
        sock->flush();

}

void CNetwork::AddFriendAck(unsigned int uid, int state, QString message)
{
        MCPACK           pack;
        memset(&pack, 0, sizeof(pack));
        pack.action = MA_ADD_FRIEND_ANSWER;
        pack.answer.state = state;
        pack.answer.uid = uid;
        strncpy(pack.answer.message, message.toStdString().c_str(), 32);

        sock->write((const char *)&pack, sizeof(pack));
        sock->flush();

}

void CNetwork::Connect()
{
        CConfig * pConfig = CConfig::Get();
        sock->close();
        sock->connectToHost(pConfig->GetServerIP(), pConfig->GetServerPort());
}

void CNetwork::Heart()
{
        MCPACK          pack;
        memset(&pack, 0, sizeof(pack));
        pack.action = MA_ZERO;
        sock->write((const char *)&pack, sizeof(pack));
        sock->flush();
}

void CNetwork::GetOnlineUser()
{
        MCPACK          pack;
        memset(&pack, 0, sizeof(pack));
        pack.action = MA_ONLINE_USER;

        sock->write((const char *)&pack, sizeof(pack));
        sock->flush();

}

















