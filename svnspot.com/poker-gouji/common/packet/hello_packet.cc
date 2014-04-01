
#include "hello_packet.h"

#include <sstream>
#include <string>
using namespace std;


void HelloPacket::Serialize(CArchive& ar)
{
    ar << ID_HELLO << player_id_;
}


void HelloPacket::Deserialize(CArchive& ar)
{
    int id;
    ar >> id;
    if (id != ID_HELLO)
        throw("Not hello packet.");
    ar >> player_id_;
}

