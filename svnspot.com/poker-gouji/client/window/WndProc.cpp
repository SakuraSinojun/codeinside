

#include <windows.h>
#include <algorithm>
#include <vector>
#include <iostream>
#include "Window.h"

#include <stdio.h>
using namespace std;


static vector<CWindow *>       vecWindows;

void _RegisterWindow(CWindow * window)
{
        vecWindows.push_back(window);
}

void _UnregisterWindow(CWindow * window)
{
        vector<CWindow *>::iterator iter;
        if(vecWindows.empty())
                return;
        iter = find(vecWindows.begin(), vecWindows.end(), window);
        if(iter == vecWindows.end())
                return;
        vecWindows.erase(iter);
}

static CWindow *   _GetWindow(HWND hWnd)
{
        CWindow *   window;
        vector<CWindow *>::iterator iter;
        for(iter=vecWindows.begin(); iter!=vecWindows.end(); iter++)
        {
                window = *iter;
                if(window == NULL)
                        continue;
                if(window->GetSafehWnd() == hWnd)
                        return window;
        }
        return NULL;
}

CWindow *   FindWindowByHWND(HWND hWnd)
{
        return _GetWindow(hWnd);
}

LRESULT CALLBACK    _WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
        // 自定义消息分给每个窗口。
        if(uMsg >= WM_USER)
        {
                CWindow *   window;
                vector<CWindow *>::iterator iter;
                for(iter=vecWindows.begin(); iter!=vecWindows.end(); iter++)
                {
                        window = *iter;
                        if(window == NULL)
                                continue;
                        window->WindowProc(window->GetSafehWnd(), uMsg, wParam, lParam);
                }
                return 0;
        }

        CWindow *   window = _GetWindow(hWnd);

        if( uMsg != WM_MOUSEMOVE && 
                uMsg != WM_SETCURSOR &&
                uMsg != WM_NCHITTEST &&
                uMsg != WM_LBUTTONDOWN && 
                uMsg != WM_LBUTTONUP
          )
        {
                // printf("%#8x, %#8x, %d\n", hWnd, uMsg, uMsg);
                // fflush(stdout);
        }

        if(window != NULL)
        {
                return window->WindowProc(hWnd, uMsg, wParam, lParam);
        }
        return DefWindowProc(hWnd, uMsg, wParam, lParam);
}


