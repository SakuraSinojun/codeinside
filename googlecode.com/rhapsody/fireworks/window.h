
/*
 *  File:		window.h
 *
 *  Author:		SakuraSinojun@gmail.com
 *
 *  Date:		2010-11-19
 *
 *  Tab Width:	6
 *
 */
 
 
#pragma once

#ifndef __WINDOW_H__
#define __WINDOW_H__

////////////////////////////////////////////////////////////////////////////////////////////////////////


#define WINVER			0x500
#define WIN32_LEAN_AND_MEAN	

#include <windows.h>

#define BKGNDCOLOR		RGB(255,255,255)
#define WINDOW_WIDTH		640
#define WINDOW_HEIGHT		480

////////////////////////////////////////////////////////////////////////////////////////////////////////



void CreateFwWindow(WNDPROC _lpfnWindowProc, HINSTANCE hInstance);

void CalcFPS();



#endif

