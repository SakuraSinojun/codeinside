
#pragma once

#include <windows.h>
#include "Window.h"

class CButton   : public CWindow
{
public:
        CButton();
        virtual ~CButton();
        virtual void PreCreateWindow(CREATEWINDOWPARAM& param);

        virtual void OnClick();
        virtual void OnCommand(WPARAM wParam, LPARAM lParam);
};

