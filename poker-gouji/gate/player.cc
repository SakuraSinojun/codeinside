


#include "player.h"
#include "game.h"


#include "Archive.h"
#include "packet/packet.h"
#include "packet/hello_packet.h"
#include "packet/server_info_packet.h"
#include "packet/play_packet.h"

#include "log.h"
DECLARE_MODULE(player);


void Player::OnReadData(const void * buf, int len)
{
    std::string xml;
    for (xml = ParsePacket(buf, len); !xml.empty(); xml = ParsePacket(NULL)) {
        DispatchPacket(xml);
    }
}


void Player::DispatchPacket(std::string xml)
{
    CArchive    ar;
    int         id;
    ar.fromStdString(xml);
    ar >> id;
    ar.fromStdString(xml);

    switch (id) {
    case Packet::ID_HELLO:
        OnHello(ar);
        break;
    case Packet::ID_SERVERINFO:
        OnServerInfo(ar);
        break;
    case Packet::ID_PLAY:
        OnPlay(ar);
        break;
    default:
        break;
    }
}

void Player::OnHello(CArchive& ar)
{
    RUN_HERE();
    HelloPacket h;
    h.Deserialize(ar);

    id_ = Game::GetInstance()->GeneratePlayerName(h.PlayerID());
    h.PlayerID() = id_;
    SendPacket(&h);
    
    Game::GetInstance()->JoinPlayer(this);
}

void Player::OnServerInfo(CArchive& ar)
{
    ServerInfoPacket    p;
    p.Deserialize(ar);
    RUN_HERE() << "for what?";
}

void Player::OnPlay(CArchive& ar)
{
    PlayPacket  p;
    p.Deserialize(ar);
    Game::GetInstance()->OnPlay(this, p.Cards());
}

void Player::SendPacket(Packet* packet)
{
    SCHECK(packet != NULL);
    if (packet == NULL)
        return;

    CArchive    ar;
    packet->Serialize(ar);
    std::string xml = ar.toStdString();
    Send(xml.c_str(), xml.length());
}



