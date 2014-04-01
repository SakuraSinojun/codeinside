#ifndef NETWORK_H
#define NETWORK_H

#include <QObject>
#include <QtNetwork/QTcpSocket>
#include <QString>

class CNetwork : public QObject
{
        Q_OBJECT
private:
        explicit CNetwork(QObject *parent = 0);

signals:

public slots:

public:
        static CNetwork * Get();

        void Connect();
        void Login(QString username, QString password);
        void GetUserinfo(unsigned int uid);
        void GetFriendList();
        void Chat(unsigned int uid, unsigned int gid, QString message);
        void AddFriend(unsigned int uid);
        void AddFriendAck(unsigned int uid, int state, QString message);
        void Heart();
        void GetOnlineUser();

        QTcpSocket      *       sock;

private:
        static CNetwork * m_handler;


};

#endif // NETWORK_H
