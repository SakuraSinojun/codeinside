

#pragma once

#include "Archive.h"

class CSerial
{
public:
        CSerial(){};
        virtual ~CSerial(){};

        // content => ar
        virtual void Serialize(CArchive& ar) = 0;

        // content <= ar
        virtual void Deserialize(CArchive& ar) = 0;

};

