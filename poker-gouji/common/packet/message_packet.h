
#pragma once

#include "packet.h"

#include <string>

class MessagePacket : public Packet {
public:
    virtual void Serialize(CArchive& ar);
    virtual void Deserialize(CArchive& ar);


    std::string& Message(void) { return message_; }

private:
    std::string message_;
    
};



