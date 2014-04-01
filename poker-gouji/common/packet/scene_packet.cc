
#include "scene_packet.h"

#include <sstream>
#include <string>
using namespace std;


void ScenePacket::Serialize(CArchive& ar)
{
    ar << ID_SCENE << cards << owner << player << m_v.size();
    std::map<std::string, std::string>::iterator iter;
    for (iter = m_v.begin(); iter != m_v.end(); iter++) {
        ar << iter->first << iter->second;
    }
}


void ScenePacket::Deserialize(CArchive& ar)
{
    int id;
    ar >> id;
    if (id != ID_SCENE)
        throw("Not Scene packet.");
    ar >> cards >> owner >> player;
    size_t size;
    ar >> size;
    size_t i;
    m_v.clear();
    for (i=0; i<size; i++) {
        std::string     f, s;
        ar >> f >> s;
        m_v[f] = s;
    }
}


