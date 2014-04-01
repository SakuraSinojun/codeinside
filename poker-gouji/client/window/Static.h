
#pragma once

#include <windows.h>
#include "Window.h"

class CStatic : public CWindow
{
public:
    CStatic();
    virtual ~CStatic();
    virtual void PreCreateWindow(CREATEWINDOWPARAM& param);
};

