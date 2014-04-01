
#pragma once

#include "packet.h"

#include <string>
#include <map>
#include <algorithm>

class ScenePacket : public Packet {
public:
    virtual void Serialize(CArchive& ar);
    virtual void Deserialize(CArchive& ar);


    std::string& Cards(void) { return cards; }
    std::string& Owner(void) { return owner; }
    std::string& Player(void) { return player; }
    
    std::string GetPlayerStates(std::string player) { 
        std::map<std::string, std::string>::iterator iter = m_v.find(player);
        if (iter != m_v.end())
            return iter->second;
        return std::string("");
    }
    void SetPlayerStates(std::string player, std::string states) { m_v[player] = states; }

    std::map<std::string, std::string>& Map(void) { return m_v; }

private:
    std::string cards;
    std::string owner;
    std::string player; 
    std::map<std::string, std::string> m_v;
};



