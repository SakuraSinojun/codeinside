
#include "deal_packet.h"

#include <sstream>
#include <string>
using namespace std;


void DealPacket::Serialize(CArchive& ar)
{
    ar << ID_DEAL << cards_;
}


void DealPacket::Deserialize(CArchive& ar)
{
    int id;
    ar >> id;
    if (id != ID_DEAL)
        throw("Not message packet.");
    ar >> cards_;
}

