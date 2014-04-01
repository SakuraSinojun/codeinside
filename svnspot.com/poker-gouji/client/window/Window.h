/*************************************************
 *
 *      所有窗口类的基类                   
 *
 * **********************************************/

#pragma once

#include <windows.h>
#include <vector>
#include "Rect.h"

// 创建窗口的参数在这个结构中
// 可以在 PreCreateWindow 中修改每个值以创建不同风格的窗口
typedef struct tagCREATEWINDOWPARAM
{
                DWORD dwExStyle;
                DWORD dwStyle;
                LPCTSTR lpClassName;
                LPCTSTR lpWindowName;
                CRect rect;
                HMENU hMenu;
                LPVOID lpParam;
                WNDCLASS WndClass;
} CREATEWINDOWPARAM;

// CWindow
// 基本窗口类，封装了一些常用API
class CWindow
{
public:
        typedef enum
        {
                eCenter_Both, eCenter_Horizontal, eCenter_Vertical,
        } ECENTER;
public:
        CWindow();
        virtual ~CWindow();
        virtual void PreTranslateMessage(const MSG* msg);
        virtual void PreCreateWindow(CREATEWINDOWPARAM& param);
        virtual void Create(CWindow * parent, CRect& rect, LPCTSTR title = TEXT(""));
        virtual HWND GetSafehWnd();
        virtual void Show(bool bShow = true);
        virtual LRESULT WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
        virtual LRESULT SendMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);
        virtual CWindow * GetParent();
        virtual int GetWindowTextLength(void);
        virtual int GetWindowText(LPTSTR text, int len);
        virtual void SetWindowText(LPCTSTR text);
        virtual void CenterWindow(ECENTER eCenter = eCenter_Both);
        virtual void GetClientRect(CRect& rect);
        virtual void GetWindowRect(CRect& rect);
        virtual void GetScreenSize(CSize& size);
        virtual void GetFullScreenSize(CSize& size);
        virtual void MoveWindow(int x, int y, int w, int h, bool bRepaint = true);
        virtual void MoveWindow(CRect rect, bool bRepaint = true);
        virtual BOOL EnableWindow(BOOL bEnable = TRUE);
        virtual bool IsVisible(void);
        virtual void InvalidateRect(CRect rect, BOOL bErase = TRUE);
        virtual void Invalidate(BOOL bErase = TRUE);
        virtual void BringWindowToTop(void);
        virtual int  doModal(void);
        virtual void Close(void);
        virtual void MsgBox(LPCTSTR text, LPCTSTR title = TEXT("Message"), UINT flag = MB_OK | MB_ICONINFORMATION); 

        virtual unsigned int  SetTimer(unsigned int uElapse, unsigned int timerID = 0);
        virtual void KillTimer(unsigned int timerid);
public:
        virtual CRect GetRect();
public:
        virtual void OnCreate();
        virtual void OnClose();
        virtual void OnDestroy();
        virtual void OnShow(bool bShow);
        virtual void OnPaint(HDC hdc);
        virtual void OnTimer(int nTimerID);
        virtual void OnCommand(WPARAM wParam, LPARAM lParam);
        virtual void OnSize(int w, int h);
        virtual void OnMouseDown(int nButton, int x, int y, int keyState);
        virtual void OnMouseUp(int nButton, int x, int y, int keyState);
        virtual void OnClick(int nButton, int x, int y, int keyState);
        virtual void OnDoubleClick(int nButton, int x, int y, int keyState);
        virtual void OnMouseMove(int x, int y, int keyState);
        virtual void OnGetMinMaxInfo(MINMAXINFO * lpmmi);
        virtual LRESULT OnEraseBackGround(WPARAM wParam, LPARAM lParam);
        virtual void OnKeyDown(int key, LPARAM lParam);
        virtual void OnChar(int key, LPARAM lParam);

private:
        static HINSTANCE m_hInstance;

        HWND m_hWnd;
        CREATEWINDOWPARAM m_CreateParam;
        bool    m_IsModal;
        bool    m_Modaling;
};

