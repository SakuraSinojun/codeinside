
#pragma once

#include <string>
#include "Archive.h"
#include "Serial.h"

class Packet : public CSerial {
public:
    enum {
        ID_HELLO = 0,
        ID_SERVERINFO,
        ID_MESSAGE,
        ID_HEARTBIT,
        ID_GAMESTART,

        ID_DEAL,
        ID_SCENE,
        ID_PLAY,

    };

    Packet(){};
    virtual ~Packet(){};

};


