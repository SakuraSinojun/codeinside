
#pragma once

#include "packet.h"

#include <string>

class DealPacket : public Packet {
public:
    virtual void Serialize(CArchive& ar);
    virtual void Deserialize(CArchive& ar);

    std::string& Cards(void) { return cards_; }

private:
    std::string cards_;
    
};



