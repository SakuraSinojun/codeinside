
#pragma once

#include "packet.h"

#include <string>

class ServerInfoPacket : public Packet {
public:
    virtual void Serialize(CArchive& ar);
    virtual void Deserialize(CArchive& ar);

    int&    Players(void) { return players_; }
    int&    MaxPlayers(void) { return max_players_; }
    std::string& ServerInfo(void) { return server_info_; }
    std::string& Message(void) { return message_; }

    ServerInfoPacket() : Packet(), players_(0), max_players_(0), server_info_("poker server"), message_("") {}
    virtual ~ServerInfoPacket() {}

private:
    int         players_;
    int         max_players_;
    std::string server_info_;
    std::string message_;
};



