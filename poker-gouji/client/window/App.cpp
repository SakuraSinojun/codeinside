
#include <windows.h>
#include "App.h"
#include <stdio.h>
#include <shlwapi.h>

#ifdef  _MSC_VER
#pragma comment(linker,"\"/manifestdependency:type='win32' \
name='Microsoft.Windows.Common-Controls' version='6.0.0.0' \
processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")
#endif

static CApp    *   m_mainApp   = NULL;


// #define _WIN32_IE       0x600
// #define _WIN32_WINNT    0x900
#include <commctrl.h>
typedef struct tagICCEX{
	DWORD dwSize;
	DWORD dwICC;
} ICCEX;
typedef BOOL (WINAPI * LPFNICCEXPROC)(ICCEX *);

CApp::CApp() :
        m_bLoop(true)
{
        m_mainApp   = this;
#if 0
        ICCEX           cc;
        cc.dwICC = ICC_LISTVIEW_CLASSES | ICC_STANDARD_CLASSES;
        cc.dwSize = sizeof(cc);
 
        HINSTANCE       hInst = ::LoadLibraryW(L"COMCTL32.DLL");
        if(hInst == NULL)
        {
                printf("Null hInst.\n");
                return;
        }
        LPFNICCEXPROC pfnInit = (LPFNICCEXPROC)::GetProcAddress(hInst, "InitCommonControlsEx");
        if (pfnInit == NULL)
        {
                FreeLibrary(hInst);
                return;
        }
        BOOL ret = InitCommonControlsEx((INITCOMMONCONTROLSEX *)&cc);
        FreeLibrary(hInst);
        printf("InitCommonControlsEx ret = %d\n", ret);
#else
        InitCommonControls();
/*
        INITCOMMONCONTROLSEX    cc;
        cc.dwICC = ICC_LISTVIEW_CLASSES | ICC_STANDARD_CLASSES;
        cc.dwSize = sizeof(cc);
        BOOL ret = InitCommonControlsEx(&cc);
        printf("InitCommonControlsEx ret = %d\n", ret);
*/
#endif

}

DWORD CApp::GetVersion(LPCTSTR lpszDllName)  
{  
    HINSTANCE hinstDll;  
    DWORD dwVersion = 0;  
  
    /* For security purposes, LoadLibrary should be provided with a fully-qualified  
       path to the DLL. The lpszDllName variable should be tested to ensure that it  
       is a fully qualified path before it is used. */  
    hinstDll = LoadLibrary(lpszDllName);  
      
    if(hinstDll)  
    {  
        DLLGETVERSIONPROC pDllGetVersion;  
        pDllGetVersion = (DLLGETVERSIONPROC)GetProcAddress(hinstDll, "DllGetVersion");  
  
        /* Because some DLLs might not implement this function, you must test for  
           it explicitly. Depending on the particular DLL, the lack of a DllGetVersion  
           function can be a useful indicator of the version. */  
  
        if(pDllGetVersion)  
        {  
            DLLVERSIONINFO2 dvi;  
            HRESULT hr;  
  
            ZeroMemory(&dvi, sizeof(dvi));  
            dvi.info1.cbSize = sizeof(dvi);  
  
            hr = (*pDllGetVersion)((DLLVERSIONINFO*)&dvi);  
  
            if(SUCCEEDED(hr))  
            {  
               dwVersion = MAKELONG(dvi.info1.dwMajorVersion, dvi.info1.dwMinorVersion);  
            }  
        }  
        FreeLibrary(hinstDll);  
    }  
    return dwVersion;  
}  

CApp::~CApp()
{
}

void CApp::Init()
{
}

void CApp::loop(void)
{
        Init();
        MSG     msgCur;
        while(m_bLoop)
        {
                if(GetMessage(&msgCur, NULL, 0, 0) == 0)
                {
                        break;
                }
                TranslateMessage(&msgCur);
                DispatchMessage(&msgCur);
        }
}

CApp * GetApp()
{
        return m_mainApp;
}

void CApp::quit(void)
{
        m_bLoop = false;
}




