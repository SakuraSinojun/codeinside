
#pragma once

#include "packet.h"

#include <string>

class HeartbitPacket : public Packet {
public:
    virtual void Serialize(CArchive& ar);
    virtual void Deserialize(CArchive& ar);
};



