
#pragma once

#include <windows.h>
#include "Window.h"


class CView     : public CWindow
{
public:
    CView();
    virtual ~CView();
    virtual void PreCreateWindow(CREATEWINDOWPARAM& param);
};

