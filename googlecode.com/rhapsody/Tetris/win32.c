

//#define __GAME_DEBUG_MESSAGE__

//#include "debug.h"
#include "Tetris.h"
#include <windows.h>
#include <assert.h>
#include <TCHAR.h>
#include <stdio.h>

//#define printf __out_debug_


#ifndef ___in
#define ___in
#define ___out
#define ___inout
#endif

#define                 BOARD_WIDTH      12
#define                 BOARD_HEIGHT     20
#define                 BOX_WIDTH        20
static const COLORREF   colors[] = 
{
        RGB(255, 0, 0),
        RGB(0, 255, 0),
        RGB(0, 0, 255),
        RGB(255, 255, 0),
        RGB(255, 0, 255),
        RGB(0, 255, 255),
        RGB(176, 224, 230),
        RGB(227, 207, 87),
        RGB(65, 105, 225),
        RGB(255, 153, 18),
        RGB(235, 142, 85),
        RGB(255, 227, 132),
        RGB(54, 94, 15),
        RGB(227, 168, 105),
        RGB(127, 255, 212),
        RGB(64, 224, 208),
        RGB(255, 125, 64),
        RGB(138, 43, 226),
};

static const int        COLOR_COUNT = sizeof(colors) / sizeof(RGB(0, 0, 0));


static HWND             m_hWnd = NULL;
static int              keymap[256];
static int              goal = 0;
static DWORD            m_LastTick = 0;
static BOOL             m_bPause = FALSE;

//////////////////////////////////////////////////////////////////////////////////////////////////

BOOL OnIdle();
void CreateLingWindow(WNDPROC _lpfnWindowProc, int width, int height, BOOL onidle);

//////////////////////////////////////////////////////////////////////////////////////////////////

BOOL file_exists(LPCTSTR file)
{	
        WIN32_FIND_DATA	fd;
        HANDLE		handle;	
                
        handle = FindFirstFile(file, &fd);
        FindClose(handle);
        
        if(handle == INVALID_HANDLE_VALUE)
        {
                return FALSE;
        }
        
        return TRUE;

}

void Board2Client(___inout POINT * point)
{
        int     cx, cy;
        RECT    rect;
        
        assert(point != NULL);
        
        GetClientRect(m_hWnd, &rect);
        
        cx = (rect.right - rect.left - BOARD_WIDTH * BOX_WIDTH) / 2;
        cy = (rect.bottom - rect.top - BOARD_HEIGHT * BOX_WIDTH) / 2;
        point->x = point->x * BOX_WIDTH + cx;
        point->y = point->y * BOX_WIDTH + cy;
}

void Client2Board(___inout POINT * point)
{
        int     cx, cy;
        RECT    rect;
        
        assert(point != NULL);
        
        GetClientRect(m_hWnd, &rect);
        
        cx = (rect.right - rect.left - BOARD_WIDTH * BOX_WIDTH) / 2;
        cy = (rect.bottom - rect.top - BOARD_HEIGHT * BOX_WIDTH) / 2;
        point->x = ( point->x - cx ) / BOX_WIDTH;
        point->y = ( point->y - cy ) / BOX_WIDTH;

}

void GetGameRect(___out RECT * rect)
{
        int     cx, cy;
        RECT    rc;
        
        assert(rect != NULL);
        
        GetClientRect(m_hWnd, &rc);
        
        cx = (rc.right - rc.left - BOARD_WIDTH * BOX_WIDTH) / 2;
        cy = (rc.bottom - rc.top - BOARD_HEIGHT * BOX_WIDTH) / 2;

        rect->left = cx;
        rect->top = cy;
        rect->right = rc.right - cx;
        rect->bottom = rc.bottom - cy;
}


void gotoxy(int x,int y)
{
        COORD C;
        C.X = x;
        C.Y = y;
        SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), C);
}


//////////////////////////////////////////////////////////////////////////////////////////////////

void CreateLingWindow(WNDPROC _lpfnWindowProc, int width, int height, BOOL onidle)
{
	HWND		hWnd;
	HINSTANCE	hInstance;
	DWORD	        dwStyle = WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX ;
	RECT	        rect;
	WNDCLASS        wndclas; 
        MSG 		msgCur; 
	BOOL		idle=TRUE;
	int		count = 0;
	
	hInstance = GetModuleHandle(NULL);
	
	
	wndclas.cbClsExtra 	= 0; 
	wndclas.cbWndExtra 	= 0; 
	wndclas.hbrBackground 	= (HBRUSH)GetStockObject(BLACK_BRUSH); 
	wndclas.hCursor 	= LoadCursor(NULL,IDC_ARROW); 
	wndclas.hIcon 		= LoadIcon(NULL,IDI_APPLICATION); 
	wndclas.hInstance 	= hInstance; 
	wndclas.lpfnWndProc 	= _lpfnWindowProc; 
	wndclas.lpszClassName	= _T("LINGWINDOW");
	wndclas.lpszMenuName	= NULL; 
	wndclas.style		= CS_VREDRAW | CS_HREDRAW | CS_OWNDC; 
	RegisterClass(&wndclas); 
	
	rect.left = 0;
	rect.top = 0;
	rect.right = width;
	rect.bottom = height;
	AdjustWindowRect(&rect, dwStyle, FALSE);
	
	hWnd = CreateWindow(_T("LINGWINDOW"),
				_T("ling"),
				dwStyle ,
				CW_USEDEFAULT,
				CW_USEDEFAULT, 
				rect.right-rect.left,
				rect.bottom-rect.top,
				NULL,
				NULL,
				hInstance,
				NULL); 
	
	ShowWindow (hWnd,SW_SHOWNORMAL); 
	
	

	if(onidle)
	{
		for(;;)
		{
			if (PeekMessage(&msgCur, NULL, 0, 0, PM_NOREMOVE))
			{
                                /*
                                if(msgCur.message == WM_USER + 0x401)
                                {
                                       //PeekMessage(&msgCur, NULL, 0, 0, PM_REMOVE);
                                        PostMessage(hWnd, WM_CLOSE, 0, 0);
                                }
                                */
				if (!GetMessage(&msgCur, NULL, 0, 0))
					return;

				//if (!PreTranslateMessage(&msgCur))
				{
					TranslateMessage(&msgCur);
					DispatchMessage(&msgCur);
				}
				idle = TRUE;
				count = 0;
			}else if (idle){
				if(count==MAXLONG)
					count=0;
				if(!OnIdle())
					idle = FALSE;
			}else{
				WaitMessage();
			}
		}
	}else{
                for(;;)
                {
                        /*
		        if(PeekMessage(&msgCur, NULL, 0, 0, PM_NOREMOVE))
                        {
                                if(msgCur.message == WM_USER + 0x401)
                                {
                                        PostMessage(hWnd, WM_CLOSE, 0, 0);
                                }
                        }
                        */
                        if(GetMessage(&msgCur, NULL, 0, 0) == 0)
                        {
                                break;
                        }
			TranslateMessage(&msgCur);
			DispatchMessage(&msgCur);
		}
	}

	return ;
}

////////////////////////////////////////////////////////////////////////////////


BOOL OnIdle()
{
	return TRUE;
}


void OnPaint(HDC hdc)
{
        BOXTYPE board[BOARD_HEIGHT][BOARD_WIDTH];
        BOXTYPE nextbox[4][4];
        int     i, j;
        RECT    rect;
        HBRUSH  hBrush;
        HDC     memDC;
        HBITMAP hBitmap;
        int     index;
        POINT   point;
        TCHAR   temp[1024];
        
        tetris_get_board((BOXTYPE*)board);
        tetris_get_next_box(nextbox);
        
        GetClientRect(m_hWnd, &rect);
        
        hBitmap = CreateCompatibleBitmap(hdc, rect.right-rect.left, rect.bottom-rect.top);
        memDC = CreateCompatibleDC(hdc);
        SelectObject(memDC, hBitmap);
        
        hBrush = (HBRUSH)GetStockObject(BLACK_BRUSH);
        SelectObject(memDC, hBrush);
        Rectangle(memDC, rect.left, rect.top, rect.right, rect.bottom);
        
        GetGameRect(&rect);
        hBrush = (HBRUSH)GetStockObject(WHITE_BRUSH);
        SelectObject(memDC, hBrush);
        Rectangle(memDC, rect.left, rect.top, rect.right, rect.bottom);
        
        
        for(j=0; j<BOARD_HEIGHT; j++)
        {
                for(i=0; i<BOARD_WIDTH; i++)
                {
                        index = board[j][i];
                        if(index != 0)
                        {
                                hBrush = CreateSolidBrush(colors[index % COLOR_COUNT]);
                                SelectObject(memDC, hBrush);
                                point.x = i;
                                point.y = j;
                                Board2Client(&point);
                                Rectangle(memDC, point.x, point.y, 
                                        point.x+BOX_WIDTH, point.y + BOX_WIDTH);
                                DeleteObject(hBrush);
                        }
                }
        }
        
        
        for(j=0; j<4; j++)
        {
                for(i=0; i<4; i++)
                {
                        index = nextbox[j][i];
                        if(index != 0)
                        {
                                hBrush = CreateSolidBrush(colors[index % COLOR_COUNT]);
                        }else{
                                hBrush = CreateSolidBrush(RGB(0, 0, 0));
                        }
                        point.x = BOARD_WIDTH + 1 + i;
                        point.y = j;
                        Board2Client(&point);
                        
                        SelectObject(memDC, hBrush);
                        Rectangle(memDC, point.x, point.y, 
                                point.x + BOX_WIDTH, point.y + BOX_WIDTH);
                        DeleteObject(hBrush);
                }
        }
        
        GetClientRect(m_hWnd, &rect);
        BitBlt(hdc, 0, 0, rect.right-rect.left, rect.bottom-rect.top, 
                memDC, 0, 0, SRCCOPY);

        DeleteDC(memDC);
        DeleteObject(hBitmap);
        
        
        gotoxy(0, 0);
        for(j=0; j<BOARD_HEIGHT; j++)
        {
                for(i=0; i<BOARD_WIDTH; i++)
                {
                        if(board[j][i] != 0)
                        {
                                printf("■");
                        }else{
                                printf("　");
                        }
                }
                printf("\n");
        }
        
        gotoxy(BOARD_WIDTH * 2 + 2, 0);
        printf("下一个方块：");
        for(j=0; j<4; j++)
        {
                gotoxy(BOARD_WIDTH * 2 + 2, j+2);
                for(i=0; i<4; i++)
                {
                        if(nextbox[j][i] != 0)
                        {
                                printf("■");
                        }else{
                                printf("　");
                        }
                }
        }
        
        gotoxy(0, BOARD_HEIGHT + 1);
        printf("总消行：%d", goal);
        wsprintf(temp, _T("总行数: %d"), goal);
        SetWindowText(m_hWnd, temp);
        
}

void OnCreate(HWND hWnd)
{
        int     i;

        m_hWnd = hWnd;
        
        for(i=0; i<256; i++)
        {
                keymap[i] = 0;
        }
        SetTimer(hWnd, 1, 500, NULL);       
        tetris_init(BOARD_WIDTH, BOARD_HEIGHT);
        tetris_use_extended_boxes(true);
}

void OnClose()
{       
        tetris_quit();
}

void OnTimer(DWORD nEventID)
{
        int     count;

        if(m_bPause && nEventID != 'P')
        {
                return;
        }

        switch(nEventID)
        {
        case 1:
                count = tetris_game_loop();
                if(count < -1)
                {
                        InvalidateRect(m_hWnd, NULL, FALSE);
                        //MessageBox(m_hWnd, _T("游戏结束"), _T(""), MB_OK);
                        tetris_quit();
                        tetris_init(BOARD_WIDTH, BOARD_HEIGHT);
                        goal = 0;
                }else{
                        
                        if(count == -1)
                        {
                                if(file_exists(_T("oned.wav")))
                                {
                                        PlaySound(_T("oned.wav"),NULL,SND_ALIAS|SND_ASYNC);
                                }
                        }else{
                                goal += count;
                                if(count > 0)
                                {
                                        if(file_exists(_T("AK47A.wav")))
                                        {
                                                PlaySound(_T("AK47A.wav"),NULL,SND_ALIAS|SND_ASYNC);
                                        }                                
                                }
                               
                        }

                }
                InvalidateRect(m_hWnd, NULL, FALSE);
                
				m_LastTick = GetTickCount();
				break;
        }
        
}

void OnKeyDown(int nKeyCode)
{
        int     count;

        if(m_bPause && nKeyCode != 'P')
	{
			return;
	}

        keymap[nKeyCode] = 1;

        if(GetTickCount() - m_LastTick > 500 && m_LastTick != 0)
        {
                OnTimer(1);
				m_LastTick = GetTickCount();
        }
        switch(nKeyCode)
        {
        case VK_UP:
                tetris_rotate(true);
                if(file_exists(_T("rotate.wav")))
                {
                        PlaySound(_T("rotate.wav"),NULL,SND_ALIAS|SND_ASYNC);
                }
                break;
        case VK_DOWN:
                tetris_move_down();
                break;
        case VK_LEFT:
                tetris_move_left();
                break;
        case VK_RIGHT:
                tetris_move_right();
                break;
        case VK_SPACE:
                count = tetris_instant_drop();
                if(count < -1)
                {
                        break;
                }
                
                if(count > 0)
                {
                        goal += count;
                        if(file_exists(_T("AK47A.wav")))
                        {
                                PlaySound(_T("AK47A.wav"),NULL,SND_ALIAS|SND_ASYNC);
                        }                                
                }else{
                        if(file_exists(_T("oned.wav")))
                        {
                                PlaySound(_T("oned.wav"),NULL,SND_ALIAS|SND_ASYNC);
                        }                        
                }
                

                break;
		case 'P':
			    m_bPause = ! m_bPause;
		        break;
        }
        InvalidateRect(m_hWnd, NULL, FALSE);
}

void OnKeyUp(int nKeyCode)
{
        keymap[nKeyCode] = 0;
}

////////////////////////////////////////////////////////////////////////////////

LRESULT CALLBACK _WndProc(HWND hwnd, UINT uMsg, WPARAM wParam,LPARAM lParam) 
{ 

        PAINTSTRUCT 	ps;
	
	switch(uMsg) 
	{
	case WM_CREATE:
		OnCreate(hwnd);
		break;
	case WM_CLOSE:
                OnClose();
		PostQuitMessage (0);
		break;
	case WM_DESTROY:
		exit(0);
		break;
        case WM_PAINT:
                BeginPaint(hwnd, &ps);
                OnPaint(ps.hdc);
                EndPaint(hwnd, &ps);
                break;
        case WM_TIMER:
                OnTimer(wParam);
                break;
        case WM_KILLFOCUS:
                break;
        case WM_SETFOCUS:
                break;
        case WM_KEYDOWN:
                OnKeyDown(wParam);
                break;
        case WM_KEYUP:
                OnKeyUp(wParam);
                break;
        case WM_ERASEBKGND:
                return 1;
	default:
	        return DefWindowProc(hwnd,uMsg,wParam,lParam); 
	}

 	return 0; 
}
 

int main()
{

	CreateLingWindow(_WndProc, 640, 480, FALSE);
	return 0;
}







