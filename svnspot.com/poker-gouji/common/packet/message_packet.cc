
#include "message_packet.h"

#include <sstream>
#include <string>
using namespace std;


void MessagePacket::Serialize(CArchive& ar)
{
    ar << ID_MESSAGE << message_;
}


void MessagePacket::Deserialize(CArchive& ar)
{
    int id;
    ar >> id;
    if (id != ID_MESSAGE)
        throw("Not message packet.");
    ar >> message_;
}

