
#pragma once
#include <windows.h>

#include "Window.h"


LRESULT CALLBACK    _WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

void _RegisterWindow(CWindow * window);

void _UnregisterWindow(CWindow * window);

CWindow *   FindWindowByHWND(HWND hWnd);


void LockCreate();
void UnlockCreate();


