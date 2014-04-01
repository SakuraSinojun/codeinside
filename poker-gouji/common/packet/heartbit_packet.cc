
#include "heartbit_packet.h"

#include <sstream>
#include <string>
using namespace std;


void HeartbitPacket::Serialize(CArchive& ar)
{
    ar << ID_HEARTBIT;
}


void HeartbitPacket::Deserialize(CArchive& ar)
{
    int id;
    ar >> id;
    if (id != ID_HEARTBIT)
        throw("Not message packet.");
}

