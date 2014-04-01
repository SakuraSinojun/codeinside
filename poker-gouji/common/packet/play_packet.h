
#pragma once

#include "packet.h"

#include <string>

class PlayPacket : public Packet {
public:
    virtual void Serialize(CArchive& ar);
    virtual void Deserialize(CArchive& ar);


    std::string& Cards(void) { return cards; }
    bool& Ensure(void) { return ensure; }

private:
    bool        ensure;
    std::string cards;
    
};



