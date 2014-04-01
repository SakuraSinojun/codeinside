

#pragma once

#include <string>
#include "tools/socket.h"
#include "packet_parser.h"
#include "Archive.h"

class Packet;
class Player : public tools::CSocket, public PacketParser {
public:
    Player() : tools::CSocket(tools::CSocket::eSocketType_TCP) {}
    ~Player() {}
    
    std::string& id() { return id_; }
    int&        seatid() { return seatid_; }

    virtual void OnReadData(const void * buf, int len);

    void    SendPacket(Packet* packet);


private:
    std::string     id_;    
    int             seatid_;

    void DispatchPacket(std::string xml);

    void OnHello(CArchive& ar);
    void OnServerInfo(CArchive& ar);
    void OnPlay(CArchive& ar);

};

