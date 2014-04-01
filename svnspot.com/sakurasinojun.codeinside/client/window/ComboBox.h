
#pragma once

#include <windows.h>
#include "Window.h"

class CComboBox : public CWindow
{
public:
        CComboBox();
        virtual ~CComboBox();
        virtual void PreCreateWindow(CREATEWINDOWPARAM& param);

        /*
public:
        virtual LRESULT AddString(LPCTSTR str);
        virtual LRESULT DelString(int index);
        virtual LRESULT GetCount(void);
        */
};

