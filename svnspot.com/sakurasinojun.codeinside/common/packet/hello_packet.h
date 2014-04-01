
#pragma once

#include "packet.h"

#include <string>

class HelloPacket : public Packet {
public:
    virtual void Serialize(CArchive& ar);
    virtual void Deserialize(CArchive& ar);


    std::string& PlayerID(void) { return player_id_; }

private:
    std::string player_id_;
    
};



