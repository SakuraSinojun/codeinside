#include <windows.h>
#include <windowsx.h>
#include "Window.h"
#include "WndProc.h"
#include <string.h>
#include <stdio.h>
#include <iostream>
#include <algorithm>
using namespace std;

HINSTANCE CWindow::m_hInstance = NULL;

CWindow::CWindow()
{
        m_hInstance = GetModuleHandle(NULL);
        memset(&m_CreateParam, 0, sizeof(m_CreateParam));
        m_CreateParam.dwExStyle = 0;
        m_CreateParam.dwStyle = WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX | WS_MAXIMIZEBOX | WS_SIZEBOX | WS_BORDER;
        m_CreateParam.WndClass.cbClsExtra = 0;
        m_CreateParam.WndClass.cbWndExtra = 0;
        m_CreateParam.WndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
        m_CreateParam.WndClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
        m_CreateParam.WndClass.hInstance = m_hInstance;
        m_CreateParam.WndClass.lpfnWndProc = _WndProc;
        m_CreateParam.WndClass.lpszClassName = TEXT("LINGWINDOW");
        m_CreateParam.WndClass.hbrBackground = (HBRUSH) GetStockObject(LTGRAY_BRUSH);
        m_CreateParam.WndClass.lpszMenuName = NULL;
        m_CreateParam.WndClass.style = CS_VREDRAW | CS_HREDRAW | CS_OWNDC | CS_DBLCLKS;
        m_IsModal = false;
}

CWindow::~CWindow()
{
        _UnregisterWindow(this);
}

void CWindow::PreTranslateMessage(const MSG* msg)
{
}

void CWindow::PreCreateWindow(CREATEWINDOWPARAM& param)
{
}

void CWindow::Create(CWindow * parent, CRect& rect, LPCTSTR title)
{
        HWND hwnd;

        if (parent == NULL)
                hwnd = NULL;
        else
                hwnd = parent->GetSafehWnd();

        PreCreateWindow(m_CreateParam);
        if (m_CreateParam.WndClass.lpfnWndProc != NULL)
        {
                RegisterClass(&m_CreateParam.WndClass);
        }

        if ((long)rect.left == (long)CW_USEDEFAULT && (long)rect.top == (long)CW_USEDEFAULT) {
            AdjustWindowRect(&rect, m_CreateParam.dwStyle, FALSE);
            this->m_CreateParam.rect = rect;
            this->m_hWnd = CreateWindowEx(m_CreateParam.dwExStyle, 
                m_CreateParam.WndClass.lpszClassName, 
                title, 
                m_CreateParam.dwStyle, 
                CW_USEDEFAULT, 
                CW_USEDEFAULT, 
                rect.right - rect.left,
                rect.bottom - rect.top,
                hwnd, 
                m_CreateParam.hMenu, 
                m_hInstance,
                m_CreateParam.lpParam);
        } else {
            AdjustWindowRect(&rect, m_CreateParam.dwStyle, FALSE);
            this->m_CreateParam.rect = rect;
            this->m_hWnd = CreateWindowEx(m_CreateParam.dwExStyle, 
                m_CreateParam.WndClass.lpszClassName, 
                title, 
                m_CreateParam.dwStyle, 
                rect.left, 
                rect.top, 
                rect.right - rect.left, 
                rect.bottom - rect.top, 
                hwnd, 
                m_CreateParam.hMenu, 
                m_hInstance,
                m_CreateParam.lpParam);
        }

        _RegisterWindow(this);
        // SendMessage(WM_CREATE, 0, 0);     // 不知道为什么会死机。
        HFONT   hFont = (HFONT)GetStockObject(DEFAULT_GUI_FONT);
        SendMessage(WM_SETFONT, (WPARAM)hFont, 1);

        this->OnCreate();
        // this->Show();
}

void CWindow::Show(bool bShow)
{
        if (bShow)
                ShowWindow(m_hWnd, SW_SHOWNORMAL);
        else
                ShowWindow(m_hWnd, SW_HIDE);
        OnShow(bShow);
}

void CWindow::OnShow(bool bShow)
{
}

void CWindow::OnCreate()
{
        // cout << "CWindow::OnCreate" << endl;
}

void CWindow::OnClose()
{
        // cout << "CWindow::OnClose" << endl;
        PostQuitMessage(0);
}

void CWindow::OnPaint(HDC hdc)
{
}

void CWindow::Close(void)
{
        this->SendMessage(WM_CLOSE, 0, 0);
}

LRESULT CWindow::OnEraseBackGround(WPARAM wParam, LPARAM lParam)
{
        return DefWindowProc(this->GetSafehWnd(), WM_ERASEBKGND, wParam, lParam);
}

void CWindow::OnTimer(int nTimerID)
{
}

unsigned int CWindow::SetTimer(unsigned int uElapse, unsigned int timerID)
{
    return ::SetTimer(this->GetSafehWnd(), timerID, uElapse, NULL);
}

void CWindow::KillTimer(unsigned int timerid)
{
    ::KillTimer(this->GetSafehWnd(), timerid);
}

HWND CWindow::GetSafehWnd()
{
    if (this != NULL)
        return m_hWnd;
    else
        return (HWND)0;
}

LRESULT CWindow::WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
        PAINTSTRUCT ps;
        CWindow * window;
        static int     x = 0, y = 0;
        static int     x1 = 0, y1 = 0;

        switch (uMsg)
        {
                case WM_CREATE:
                        // OnCreate();      不在这时候调用OnCreate了。
                        break;
                case WM_CLOSE:
                        OnClose();
                        if(m_IsModal)
                        {
                                m_Modaling = false;
                        }
                        break;
                case WM_DESTROY:
                        OnDestroy();
                        break;
                case WM_ERASEBKGND:
                        return OnEraseBackGround(wParam, lParam);
                        break;
                case WM_PAINT:
                        BeginPaint(hWnd, &ps);
                        OnPaint(ps.hdc);
                        EndPaint(hWnd, &ps);
                        break;
                case WM_TIMER:
                        OnTimer(wParam);
                        break;
                case WM_SIZE:
                        OnSize(LOWORD(lParam), HIWORD(lParam));
                        break;
                case WM_COMMAND:
                        window = FindWindowByHWND((HWND) lParam);
                        if (window != NULL)
                                window->OnCommand(wParam, lParam);
                        break;
                case WM_LBUTTONDOWN:
                        x = GET_X_LPARAM(lParam);
                        y = GET_Y_LPARAM(lParam);
                        OnMouseDown(0, x, y, wParam);
                        break;
                case WM_LBUTTONUP:
                        x1 = GET_X_LPARAM(lParam);
                        y1 = GET_Y_LPARAM(lParam);
                        if(abs(x - x1) <= 5 && abs(y - y1) <= 5)
                        {
                                OnClick(0, x, y, wParam);
                        }
                        OnMouseUp(0, x1, y1, wParam);
                        x = 0; y = 0; x1 = 0; y1 = 0;
                        break;
                case WM_RBUTTONDOWN:
                        OnMouseDown(1, GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam), wParam);
                        break;
                case WM_MBUTTONDOWN:
                        OnMouseDown(2, GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam), wParam);
                        break;
                case WM_MOUSEMOVE:
                        OnMouseMove(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam), wParam);
                        break;
                case WM_LBUTTONDBLCLK:
                        OnDoubleClick(0, GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam), wParam);
                        break;
                case WM_RBUTTONDBLCLK:
                        OnDoubleClick(1, GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam), wParam);
                        break;
                case WM_MBUTTONDBLCLK:
                        OnDoubleClick(2, GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam), wParam);
                        break;
                case WM_GETMINMAXINFO:
                        OnGetMinMaxInfo((MINMAXINFO *) lParam);
                        break;
                case WM_KEYDOWN:
                        OnKeyDown(wParam, lParam);
                        break;
                case WM_CHAR:
                        OnChar(wParam, lParam);
                        break;
                default:
                        break;
        }
        return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

void CWindow::OnSize(int w, int h)
{
}

void CWindow::OnCommand(WPARAM wParam, LPARAM lParam)
{
}

CRect CWindow::GetRect()
{
        return m_CreateParam.rect;
}

LRESULT CWindow::SendMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
        return ::SendMessage(this->GetSafehWnd(), uMsg, wParam, lParam);
}

CWindow * CWindow::GetParent()
{
        HWND hWnd;
        hWnd = ::GetParent(this->GetSafehWnd());
        return FindWindowByHWND(hWnd);
}

int CWindow::GetWindowTextLength()
{
        return static_cast<int>(::GetWindowTextLength(m_hWnd));
}

int CWindow::GetWindowText(LPTSTR text, int length)
{
        return ::GetWindowText(m_hWnd, text, length);
}

void CWindow::SetWindowText(LPCTSTR text)
{
        ::SetWindowText(m_hWnd, text);
}

void CWindow::GetClientRect(CRect& rect)
{
        ::GetClientRect(m_hWnd, &rect);
}

void CWindow::GetFullScreenSize(CSize& size)
{
        int w, h;
        w = ::GetSystemMetrics(SM_CXFULLSCREEN);
        h = ::GetSystemMetrics(SM_CYFULLSCREEN);
        size = CSize(w, h);
}

void CWindow::GetScreenSize(CSize& size)
{
        int w, h;
        w = ::GetSystemMetrics(SM_CXSCREEN);
        h = ::GetSystemMetrics(SM_CYSCREEN);
        size = CSize(w, h);
}

void CWindow::GetWindowRect(CRect& rect)
{
        ::GetWindowRect(m_hWnd, &rect);
}

void CWindow::CenterWindow(ECENTER eCenter)
{
        CRect rect1;
        CRect rect2;
        CSize size;
        CRect rect;
        CWindow * parent;

        parent = GetParent();
        GetClientRect(rect1);
        if (parent == NULL)
        {
                GetFullScreenSize(size);
        } else
        {
                parent->GetClientRect(rect2);
                size.cx = rect2.Width();
                size.cy = rect2.Height();
        }

        rect = rect1;
        switch (eCenter)
        {
                case eCenter_Horizontal:
                        rect.left = (size.cx - rect1.Width()) / 2;
                        rect.right = rect.left + rect1.Width();
                        break;
                case eCenter_Vertical:
                        rect.top = (size.cy - rect1.Height()) / 2;
                        rect.bottom = rect.top + rect1.Height();
                        break;
                default:
                        rect.left = (size.cx - rect1.Width()) / 2;
                        rect.right = rect.left + rect1.Width();
                        rect.top = (size.cy - rect1.Height()) / 2;
                        rect.bottom = rect.top + rect1.Height();
                        break;
        }

        MoveWindow(rect);

}

void CWindow::MoveWindow(int x, int y, int w, int h, bool bRepaint)
{
        ::MoveWindow(m_hWnd, x, y, w, h, (BOOL) bRepaint);
        m_CreateParam.rect = CRect(x, y, w + x, h + y);
}

void CWindow::MoveWindow(CRect rect, bool bRepaint)
{
        MoveWindow(rect.left, rect.top, rect.Width(), rect.Height(), bRepaint);
}

void CWindow::OnMouseDown(int nButton, int x, int y, int keyState)
{
}

void CWindow::OnMouseUp(int nButton, int x, int y, int keyState)
{
}

void CWindow::OnClick(int nButton, int x, int y, int keyState)
{
}

void CWindow::OnDoubleClick(int nButton, int x, int y, int keyState)
{
}

void CWindow::OnMouseMove(int x, int y, int keyState)
{
}

void CWindow::OnGetMinMaxInfo(MINMAXINFO * lpmmi)
{
}

BOOL CWindow::EnableWindow(BOOL bEnable)
{
        return ::EnableWindow(m_hWnd, bEnable);
}

bool CWindow::IsVisible(void)
{
        return (::IsWindowVisible(m_hWnd) != FALSE)?true:false;
}

void CWindow::InvalidateRect(CRect rect, BOOL bErase)
{
        ::InvalidateRect(m_hWnd, &rect, bErase);
}

void CWindow::Invalidate(BOOL bErase)
{
        CRect    rect;
        GetClientRect(rect);
        ::InvalidateRect(m_hWnd, &rect, bErase);
}

void CWindow::BringWindowToTop(void)
{
        ::BringWindowToTop(m_hWnd);
}

void CWindow::OnDestroy()
{
}

int CWindow::doModal(void)
{
        MSG     msgCur;
        m_IsModal = true;
        m_Modaling = true;
        while(m_Modaling)
        {
                if(GetMessage(&msgCur, NULL, 0, 0) == 0)
                {
                        break;
                }
                if(!IsDialogMessage(this->GetSafehWnd(), &msgCur))
                {
                        if(msgCur.message == WM_PAINT || msgCur.message >= WM_USER)
                        {
                                TranslateMessage(&msgCur);
                                DispatchMessage(&msgCur); 
                        }
                }
        }
        m_IsModal = false;
        return 0;
}

void CWindow::MsgBox(LPCTSTR text, LPCTSTR title, UINT flag)
{
        ::MessageBox(this->GetSafehWnd(), text, title, flag);
}

void CWindow::OnKeyDown(int key, LPARAM lParam)
{
}

void CWindow::OnChar(int key, LPARAM lParam)
{
}
