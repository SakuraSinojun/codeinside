

/*
 *  File:		firework.cpp
 *
 *  Author:		SakuraSinojun@gmail.com
 *
 *  Date:		2010-11-19 Created.
 *
 *  Description: 	This is a simple demo of firework.
 *
 *  Tab Width:	6
 *
 */


////////////////////////////////////////////////////////////////////////////////////////////////////////


#define	WINVER	0x500
#define	WIN32_LEAN_AND_MEAN


#include <windows.h>
#include <vector>
#include <MMSystem.h>
#include <math.h>

#include "window.h"
#include "fire.h"

using namespace std;

////////////////////////////////////////////////////////////////////////////////////////////////////////

//变量声明：
HWND			m_hWnd ;
HINSTANCE		m_hInstance ;
vector<CFire>	vct;

BYTE	*		bm_bg;
HBITMAP		m_hBitmap;
BITMAP		m_bitmap;

struct
{
	BITMAPINFOHEADER		Info;
	DWORD				BitField[3];
} Header;	


//函数声明：
void OnCreate	(HWND hWnd);
void OnClick	(int x, int y);
void OnTimer	(UINT nEventID);
void PaintDemo	();
void Fade 		();
void PlaySound	(const char * filename);

////////////////////////////////////////////////////////////////////////////////////////////////////////

LRESULT CALLBACK _WndProc(HWND hwnd, UINT uMsg, WPARAM wParam,LPARAM lParam) 
{ 

	PAINTSTRUCT 	ps;

	switch(uMsg) 
	{
	case WM_CREATE:
		m_hWnd = hwnd ;
		OnCreate(hwnd);
		break;
	
	case WM_PAINT:
		BeginPaint (hwnd, &ps);
		EndPaint (hwnd,&ps);
		break;
	
	case WM_LBUTTONDOWN:
		OnClick (LOWORD(lParam), HIWORD(lParam));
		break;
	
	case WM_TIMER:
		OnTimer (wParam);
		break;
		
	case WM_LBUTTONUP:
		break;
		
	case WM_CLOSE: 
		DestroyWindow(hwnd); 
		break; 
	
	case WM_DESTROY: 
		PostQuitMessage(0); 
		break; 
	
	case WM_ERASEBKGND:
		return true;
		break;
		
	default:  
		return DefWindowProc(hwnd,uMsg,wParam,lParam); 
	} 
 	return 0; 
}
 

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	m_hWnd = NULL;
	m_hInstance = hInstance;
	
	CreateFwWindow (_WndProc, hInstance);
 
	return 0;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////

void OnCreate(HWND hWnd)
{

	HDC		hdc = GetDC(hWnd);
	//UINT 		iPlanes = GetDeviceCaps(hdc, PLANES);		//调色盘
	//UINT 		iBitsPixel = GetDeviceCaps(hdc, BITSPIXEL);	//比特每像素
	int		depth;							
	int		bytes_per_line;
	
	
	depth = 24;
	bytes_per_line = ((640 * (depth + 7) / 8 + 3) / 4) * 4;		//每行字节数
	
	Header.Info.biSize		= sizeof(BITMAPINFOHEADER);
	Header.Info.biWidth		= 640;
	Header.Info.biHeight		= 480;
	Header.Info.biBitCount		= depth;
	Header.Info.biPlanes		= 1;
	Header.Info.biXPelsPerMeter	= 0;
	Header.Info.biYPelsPerMeter	= 0;
	Header.Info.biClrUsed		= 0;
	Header.Info.biClrImportant	= 0;
	Header.Info.biCompression	= depth == 24? BI_RGB: BI_BITFIELDS;
	Header.Info.biSizeImage		= bytes_per_line * 480;

	switch (depth) {
	  case 16:
		Header.BitField[0] = 0x7c00;
		Header.BitField[1] = 0x03e0;
		Header.BitField[2] = 0x001f;
		break;

	  case 32:
		Header.BitField[0] = 0xff0000;
		Header.BitField[1] = 0x00ff00;
		Header.BitField[2] = 0x0000ff;
		break;

	  default:
		Header.BitField[0] = 0;
		Header.BitField[1] = 0;
		Header.BitField[2] = 0;
		break;
	}

	m_hBitmap = CreateDIBSection(hdc, (BITMAPINFO *)&Header, DIB_RGB_COLORS, (void **)&bm_bg, NULL, 0);
	
	GetObject(m_hBitmap, sizeof(BITMAP), &m_bitmap);
	
	::ReleaseDC(0, hdc);
	
	SetTimer(hWnd, 1001, 1000, NULL);
	
}


void OnClick (int x, int y)
{
	vector<CFire>::iterator		it;
	
	PlaySound("syu.wav");
	
	for(it=vct.begin(); it!=vct.end(); it++)
	{
		if( (*it).IsStopped() )
		{
			(*it).Init(x, y);
			return;
		}
	}
	
	CFire fire(x, y);
	vct.push_back(fire);

}


void OnTimer(UINT nEventID)
{

	if(nEventID != 1001)
	{
		return;
	}
	
	srand((unsigned)GetTickCount());
	int x = 100 + rand() % ( WINDOW_WIDTH - 200 );
	int y = rand() % ( WINDOW_HEIGHT - 100 );
	int u = 2000 + rand() % 2000;
	
	OnClick(x, y);
	SetTimer (m_hWnd, 1001, u, NULL);
	
	
}


////////////////////////////////////////////////////////////////////////////////////////////////////////


void PaintDemo()
{	
	vector<CFire>::iterator		it;
	HDC					memDC;
	HDC					hdc;
	HBRUSH				hBrush;
	HBITMAP				hBitmap;
	FireStar	*			fs;
	
	
	hdc = GetDC(m_hWnd);
	memDC = CreateCompatibleDC(hdc);
	hBitmap = CreateCompatibleBitmap(hdc, WINDOW_WIDTH, WINDOW_HEIGHT);
	hBrush = (HBRUSH)GetStockObject(BLACK_BRUSH);
	SelectObject(memDC, m_hBitmap);
	SelectObject(memDC, hBrush);
	
	Rectangle(memDC, 0, 0, WINDOW_WIDTH, 2);
	
	for(it=vct.begin(); it!=vct.end(); it++)
	{
		
		if((*it).IsStopped())
		{
			continue;
		}
		
		fs = (*it).GetFirstStar();
		if(fs == NULL)
		{
			break;
		}
		SetPixel(memDC, (int)fs->x, (int)fs->y, RGB(fs->r, fs->g, fs->b));
		
		if(fs->FW_STEP == 0)
		{
			while ( (fs = (*it).GetNextStar()) != NULL )
			{
				SetPixel(memDC, (int)fs->x, (int)fs->y, RGB(fs->r, fs->g, fs->b));
			}
		}else if(fs->FW_STEP == 1)
		{
			//播放声音
			PlaySound("firework.wav");
			
		}else{
			//发射过程.

		}
		
		//(*it).NextFrame();
		(*it).CalcFrame();
	}
	
	Fade();
	
	
	BitBlt(hdc, 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, memDC, 0, 0, SRCCOPY);
	
	DeleteObject(hBitmap);
	DeleteDC(memDC);
	ReleaseDC(m_hWnd, hdc);
	
}

BOOL OnIdle(LONG count)
{
	CalcFPS();
	
	PaintDemo();
	
	return TRUE;
}




inline void Fade ()
{
	int	length;
	int	i;
	int	depth;
	int	r[4];
	int	g[4];
	int	b[4];
	int	r1, r2;
	int	g1, g2;
	int	b1, b2;
	int	sv = 64;
	int	su = 32;
	
	depth = m_bitmap.bmBitsPixel / 8;
	length = m_bitmap.bmWidth * (m_bitmap.bmHeight-1) * depth;
	
	for(i=0; i<length-depth; i+=depth)
	{
		b[0] = bm_bg[i];
		g[0] = bm_bg[i+1];
 		r[0] = bm_bg[i+2];
		b[1] = bm_bg[i+depth];
		g[1] = bm_bg[i+depth+1];
		r[1] = bm_bg[i+depth+2];
		
		
		b[2] = bm_bg[i + m_bitmap.bmWidthBytes];
		g[2] = bm_bg[i + m_bitmap.bmWidthBytes + 1];
		r[2] = bm_bg[i + m_bitmap.bmWidthBytes + 2];
		b[3] = bm_bg[i + m_bitmap.bmWidthBytes + depth];
		g[3] = bm_bg[i + m_bitmap.bmWidthBytes + depth + 1];
		r[3] = bm_bg[i + m_bitmap.bmWidthBytes + depth + 2];
		
		r1 = ( (r[1] - r[0]) * sv >> 8 ) + r[0];
		g1 = ( (g[1] - g[0]) * sv >> 8 ) + g[0];
		b1 = ( (b[1] - b[0]) * sv >> 8 ) + b[0];
		
		
		r2 = ( (r[3] - r[2]) * sv >> 8 ) + r[2];
		g2 = ( (g[3] - g[2]) * sv >> 8 ) + g[2];
		b2 = ( (b[3] - b[2]) * sv >> 8 ) + b[2];
		
		r1 = ( (r2 - r1) * su >> 8 ) + r1;
		g1 = ( (g2 - g1) * su >> 8 ) + g1;
		b1 = ( (b2 - b1) * su >> 8 ) + b1;
		
		
		*(bm_bg + i) = b1;
		*(bm_bg + i + 1) = g1;
		*(bm_bg + i + 2) = r1;
	}
	
}

void PlaySound	(const char * filename)
{
	
	//sndPlaySound(filename, SND_ASYNC);
	
	MCI_OPEN_PARMS	open;
	MCI_PLAY_PARMS	play;
	static UINT	id = 0;
	
	if (id != 0)
	{
		mciSendCommand(id, MCI_STOP, MCI_WAIT, 0);
		mciSendCommand(id, MCI_CLOSE, MCI_WAIT, 0);
	}
	
	open.lpstrDeviceType = "waveaudio";
	open.lpstrElementName = filename;
	if ( mciSendCommand(0, MCI_OPEN, MCI_OPEN_TYPE | MCI_OPEN_ELEMENT | MCI_WAIT, (DWORD)&open) ) 
	{
		return;
	}
	id = open.wDeviceID;
	
	play.dwCallback = (DWORD)m_hWnd;
	
	if ( mciSendCommand(id, MCI_PLAY, MCI_NOTIFY, (DWORD)&play) )
	{
		mciSendCommand(id, MCI_CLOSE, MCI_WAIT, 0);
		return;
	}
	
}	










