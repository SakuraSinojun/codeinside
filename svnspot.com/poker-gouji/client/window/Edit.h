
#pragma once

#include <windows.h>
#include "Window.h"

class CEdit : public CWindow
{
public:
        CEdit();
        virtual ~CEdit();
        virtual void PreCreateWindow(CREATEWINDOWPARAM& param);

public:
        virtual LRESULT GetText(LPTSTR buf, int len);
        virtual void    SetText(LPCTSTR text);
        virtual LRESULT GetTextLength(void);
        virtual void    SetMultiLine(bool multi = true);

private:
        bool    m_multiline;

};

