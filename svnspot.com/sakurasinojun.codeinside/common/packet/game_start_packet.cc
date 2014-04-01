
#include "game_start_packet.h"

#include <sstream>
#include <string>
using namespace std;


void GameStartPacket::Serialize(CArchive& ar)
{
    ar << ID_GAMESTART << m_v.size();
    size_t  i;
    for (i=0; i<m_v.size(); i++) {
        ar << m_v[i];
    }
}


void GameStartPacket::Deserialize(CArchive& ar)
{
    int id;
    ar >> id;
    if (id != ID_GAMESTART)
        throw("Not message packet.");
    ar >> id;
    int i;
    m_v.clear();
    for (i=0; i<id; i++) {
        std::string t;
        ar >> t;
        m_v.push_back(t);
    }
}


