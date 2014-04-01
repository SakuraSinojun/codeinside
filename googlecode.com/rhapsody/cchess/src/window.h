

#pragma once

#define WIN32_LEAN_AND_MEAN	

#include <windows.h>



#define BKGNDCOLOR	RGB(255,255,255)
#define WINDOW_WIDTH	617
#define WINDOW_HEIGHT	687

void CreateMyWindow(WNDPROC _lpfnWindowProc, HINSTANCE hInstance);

BOOL PointInRect (POINT point, RECT rect) ;



