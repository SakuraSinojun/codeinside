
#pragma once

#include "packet.h"

#include <string>
#include <vector>

class GameStartPacket : public Packet {
public:
    virtual void Serialize(CArchive& ar);
    virtual void Deserialize(CArchive& ar);

    int     Seats() { return m_v.size(); }
    void    Clear() { m_v.clear(); }
    void    AddSeat(std::string id) { m_v.push_back(id); }
    std::string At(int index) {
        if (index < (int)m_v.size())
            return m_v[index];
        return std::string("");
    }

private:
    std::vector<std::string>    m_v;
};



