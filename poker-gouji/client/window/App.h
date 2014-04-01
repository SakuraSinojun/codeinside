
#pragma once

#include <windows.h>

class CApp
{
public:
        CApp();
        ~CApp();
        virtual void    Init(void);
public:
        virtual void    loop(void);
        virtual void    quit(void);
        DWORD GetVersion(LPCTSTR lpszDllName);
private:
        bool    m_bLoop;
};

CApp * GetApp();
