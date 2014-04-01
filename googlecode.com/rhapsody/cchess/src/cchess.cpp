
#define WINVER	0x500
#define WIN32_LEAN_AND_MEAN

#include <windows.h>
#include "window.h"
#include "libcchess.h"
#include "chinesechess.h"
#include "stdio.h"
#include "preload.h"
#include "WinGdi.h"

////////////////////////////////////////////////////////////////////////////////////////////////////////

//变量声明：
HWND			m_hWnd ;
HINSTANCE		m_hInstance ;
CChineseChess	cc;


//函数声明：
void OnCreate	(HWND hWnd) ;
void OnPaint	(HDC hdc);
void OnTimer	(UINT nEventID);
void OnMouseMove	(int x, int y);
void OnClick	(int x, int y);

void point_to_board	(POINT& point);
void board_to_point	(POINT& point);

//常数:
#define	BMPATH_CHESSBOARD		"data\\chessboard.bmp"
#define	BMPATH_ROOK_RED		"data\\rook.bmp"
#define	BMPATH_ROOK_BLACK		"data\\rook2.bmp"
#define	BMPATH_KNIGHT_RED		"data\\knight.bmp"
#define	BMPATH_KNIGHT_BLACK	"data\\knight2.bmp"
#define	BMPATH_BISHOP_RED		"data\\bishop.bmp"
#define	BMPATH_BISHOP_BLACK	"data\\bishop2.bmp"
#define	BMPATH_MANDARIN_RED	"data\\MANDARIN.bmp"
#define	BMPATH_MANDARIN_BLACK	"data\\MANDARIN2.bmp"
#define	BMPATH_KING_RED		"data\\KING.bmp"
#define	BMPATH_KING_BLACK		"data\\KING2.bmp"
#define	BMPATH_CANNON_RED		"data\\CANNON.bmp"
#define	BMPATH_CANNON_BLACK	"data\\CANNON2.bmp"
#define	BMPATH_PAWN_RED		"data\\PAWN.bmp"
#define	BMPATH_PAWN_BLACK		"data\\PAWN2.bmp"

////////////////////////////////////////////////////////////////////////////////////////////////////////

LRESULT CALLBACK _WndProc(HWND hwnd, UINT uMsg, WPARAM wParam,LPARAM lParam) 
{ 
	PAINTSTRUCT 	ps;
	//static LPARAM	buttonpoint;
	//RECT			rect;
	//POINT			point;
	
	switch(uMsg) 
	{
	case WM_CREATE:
		m_hWnd = hwnd ;
		OnCreate(hwnd);
		break;
	
	case WM_PAINT:
		BeginPaint (hwnd, &ps);
		OnPaint (ps.hdc) ;
		EndPaint (hwnd,&ps);
		break;
		
	case WM_TIMER:
		OnTimer (wParam);
		break;
	
	case WM_MOUSEMOVE:
		OnMouseMove (LOWORD(lParam), HIWORD(lParam)) ;
		//buttonpoint = (DWORD)(-1l) ;
		break;
	
	case WM_LBUTTONDOWN:
		OnClick (LOWORD(lParam), HIWORD(lParam));
		//buttonpoint = lParam ;
		break;
	
	case WM_LBUTTONUP:
		/*
		rect.left = LOWORD(lParam)-2;
		rect.top = HIWORD(lParam)-2;
		rect.right = rect.left + 4;
		rect.bottom = rect.top + 4;
		point.x = LOWORD(buttonpoint);
		point.y = HIWORD(buttonpoint);
		
		//if (buttonpoint == lParam)
		if(PtInRect(&rect, point))
		{
			OnClick (LOWORD(lParam), HIWORD(lParam));
		}
		*/
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
	m_hInstance = NULL;
	
	CreateMyWindow (_WndProc, hInstance);
 
	return 0;
}



////////////////////////////////////////////////////////////////////////////////////////////////////////

void point_to_board (POINT& point)
{
	int x = point.x;
	int y = point.y;
	
	x = (int) x/70;
	y = (int) y/70;
	point.x = x;
	point.y = y;
}

void board_to_point (POINT& point)
{
	point.x = point.x * 70;
	point.y = point.y * 70;
	
}

////////////////////////////////////////////////////////////////////////////////////////////////////////

void OnCreate(HWND hWnd)
{	
	cc.InitChessBoard();
	cc.StartTurn(CAMP_RED);
}

HDC load_piece_bmp(int PieceTag, int Camp, BITMAP& bitmap)
{
	if(Camp == CAMP_RED)
	{
		switch (PieceTag)
		{
		case TAG_ROOK:
			return pl::Load(BMPATH_ROOK_RED, &bitmap);
			break;
		case TAG_KNIGHT:
			return pl::Load(BMPATH_KNIGHT_RED, &bitmap);
			break;
		case TAG_BISHOP:
			return pl::Load(BMPATH_BISHOP_RED, &bitmap);
			break;
		case TAG_MANDARIN:
			return pl::Load(BMPATH_MANDARIN_RED, &bitmap);
			break;
		case TAG_KING:
			return pl::Load(BMPATH_KING_RED, &bitmap);
			break;
		case TAG_CANNON:
			return pl::Load(BMPATH_CANNON_RED, &bitmap);
			break;
		case TAG_PAWN:
			return pl::Load(BMPATH_PAWN_RED, &bitmap);
			break;
		default:
			return (HDC)NULL;
			break;
		}
	}else if(Camp == CAMP_BLACK){
		switch (PieceTag)
		{
		case TAG_ROOK:
			return pl::Load(BMPATH_ROOK_BLACK, &bitmap);
			break;
		case TAG_KNIGHT:
			return pl::Load(BMPATH_KNIGHT_BLACK, &bitmap);
			break;
		case TAG_BISHOP:
			return pl::Load(BMPATH_BISHOP_BLACK, &bitmap);
			break;
		case TAG_MANDARIN:
			return pl::Load(BMPATH_MANDARIN_BLACK, &bitmap);
			break;
		case TAG_KING:
			return pl::Load(BMPATH_KING_BLACK, &bitmap);
			break;
		case TAG_CANNON:
			return pl::Load(BMPATH_CANNON_BLACK, &bitmap);
			break;
		case TAG_PAWN:
			return pl::Load(BMPATH_PAWN_BLACK, &bitmap);
			break;
		default:
			return (HDC)NULL;
			break;
		}
	}else{
		
		return (HDC)NULL;
	}
}

void OnPaint(HDC hdc)
{
	HDC		memDC;
	HBITMAP	hBitmap;
	RECT		rect;
	HDC		dc;
	BITMAP	bitmap;
	CHESSBOARD	bd;
	POINT		point;
	int		i;
	int		j;
	COLORREF 	color;
	
	if(!IsWindow(m_hWnd))
	{
		return;
	}
	
	cc.GetChessBoard(bd);
	
	GetClientRect(m_hWnd, &rect);
	memDC = CreateCompatibleDC(hdc);
	hBitmap = CreateCompatibleBitmap(hdc, rect.right-rect.left, rect.bottom-rect.top);
	SelectObject(memDC, hBitmap);
	
	dc = pl::Load(BMPATH_CHESSBOARD, &bitmap);
	BitBlt(memDC, 0, 0, bitmap.bmWidth, bitmap.bmHeight, dc, 0, 0, SRCCOPY);
	
	for(i=0; i<9; i++)
	{
		for(j=0; j<10; j++)
		{
			if(bd.sqr[i][j].hasPiece)
			{
				point.x = i;
				point.y = j;
				board_to_point (point);
				if(bd.sqr[i][j].Selected)
				{
					dc = pl::Load("DATA\\border.bmp", &bitmap);
					//BitBlt(memDC, point.x-2, point.y-2, bitmap.bmWidth, bitmap.bmHeight, dc, 0, 0, SRCCOPY);
					color =GetPixel (dc, 1, 1);
					TransparentBlt (memDC, point.x-2, point.y-2, bitmap.bmWidth, bitmap.bmHeight, dc, 0, 0, bitmap.bmWidth, bitmap.bmHeight, color);
				}
				dc = load_piece_bmp(bd.sqr[i][j].PieceTag, bd.sqr[i][j].Camp, bitmap);
				
				color =GetPixel (dc, 1, 1);
				TransparentBlt (memDC, point.x, point.y, bitmap.bmWidth, bitmap.bmHeight, dc, 0, 0, bitmap.bmWidth, bitmap.bmHeight, color);
			}
		}
	}
	
	
	BitBlt(hdc, 0, 0, rect.right-rect.left, rect.bottom-rect.top, memDC, 0, 0, SRCCOPY);
	
	DeleteObject(hBitmap);
	DeleteDC(memDC);
	
}


void OnClick (int x, int y)
{
	POINT point;
	RECT	rect;
	point.x = x;
	point.y = y;
	point_to_board(point);
	cc.Click(point.x, point.y);
	
	GetClientRect(m_hWnd, &rect);
	InvalidateRect(m_hWnd, &rect, FALSE);
	
	if(cc.WhoWin() == CW_RED)
	{
		MessageBox(m_hWnd, "红方胜!", "胜利", MB_OK);
		cc.InitChessBoard();
		cc.StartTurn(CAMP_RED);
		GetClientRect(m_hWnd, &rect);
		InvalidateRect(m_hWnd, &rect, FALSE);
	}else if(cc.WhoWin() == CW_BLACK){
		MessageBox(m_hWnd, "黑方胜!", "胜利", MB_OK);
		cc.InitChessBoard();
		cc.StartTurn(CAMP_RED);
		GetClientRect(m_hWnd, &rect);
		InvalidateRect(m_hWnd, &rect, FALSE);
	}else{
	}
}


void OnMouseMove	(int x, int y)
{
	POINT point;
	point.x = x;
	point.y = y;
	point_to_board(point);
	char temp[100];
	wsprintf(temp, "%d, %d", point.x, point.y);
	SetWindowText(m_hWnd, temp);
	
}



void OnTimer(UINT nEventID)
{
}


/*
bool _epCallBack(CPiece * piece)
{

	printf("TAGID=%d, x=%d, y=%d, camp=%d\n", piece->GetTagID(), piece->m_x, piece->m_y, piece->GetCamp());
	
	return true;
}


	TAG_ROOK,
	TAG_KNIGHT,
	TAG_BISHOP,
	TAG_MANDARIN,
	TAG_KING,
	TAG_CANNON,
	TAG_PAWN


char * GetDescByTag(int Tag)
{
	
	switch (Tag)
	{
	case TAG_ROOK:
		return "車";
		break;
	case TAG_KNIGHT:
		return "馬";
		break;
	case TAG_BISHOP:
		return "相";
		break;
	case TAG_MANDARIN:
		return "仕";
		break;
	case TAG_KING:
		return "帥";
		break;
	case TAG_CANNON:
		return "炮";
		break;
	case TAG_PAWN:
		return "兵";
		break;
	default:
		return "　";
		break;
	}
}

void DrawBoard(CChessBoard& board)
{
	int i;
	int j;
	CPiece * piece;
	
	for(i=0; i<10; i++)
	{
		for(j=0; j<9; j++)
		{
			piece = board.GetPiece(j, i);
			if(piece != NULL)
			{
				printf(GetDescByTag(piece->GetTagID()));
			}else{
				printf("囗");
			}
		}
		printf("\n");
		if(i==4)
		{
			printf("　楚　河　汉　界　\n");
		}
	}
}

*/
















