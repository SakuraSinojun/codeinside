
#include "play_packet.h"

#include <sstream>
#include <string>
using namespace std;


void PlayPacket::Serialize(CArchive& ar)
{
    ar << ID_PLAY << ensure << cards;
}


void PlayPacket::Deserialize(CArchive& ar)
{
    int id;
    ar >> id;
    if (id != ID_PLAY)
        throw("Not Play packet.");
    ar >> ensure >> cards;
}

