
#include "server_info_packet.h"

#include <sstream>
#include <string>
using namespace std;


void ServerInfoPacket::Serialize(CArchive& ar)
{
    ar << ID_SERVERINFO;
    ar << server_info_ << players_ << max_players_ << message_;
}


void ServerInfoPacket::Deserialize(CArchive& ar)
{
    int     id;
    ar >> id;
    if (id != ID_SERVERINFO)
        throw ("Not ServerInfo Packet!");
    ar >> server_info_ >> players_ >> max_players_ >> message_;
}

