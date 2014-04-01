
/*
 *  PieceKing.cpp
 *
 *  King Piece Solution
 *
 *  Copyright (C) 2010	SakuraSinojun@gmail.com
 *
 *  2010-11-12 Created.
 *
 *  Description: This file includes the moving-rule about King pieces. 
 *
 */


#include "libcchess.h"

#include "math.h"

#include <windows.h>

CPieceKing::CPieceKing(CChessBoard * board)
{
	m_board = board;
	m_tagid = TAG_KING;
	board->AddPiece(this);
	m_x = 3;
	m_y = 0;
}

CPieceKing::~CPieceKing()
{
}

int CPieceKing::MoveTest(int x, int y)
{
	CPiece * piece = NULL;
	CPiece * p1;
	int i;
	int ey;
	int min, max;
	bool flag;
	
	if(!CheckPoint(x, y))
	{
		return PM_UNREACHABLE;
	}
	
	if(m_x == x && m_y == y)
	{
		return PM_UNREACHABLE;
	}
	
	if ( x!=m_x && y!=m_y )
	{
		//不许斜着走...
		return PM_UNREACHABLE;
	}
	
	int cx = abs(m_x - x);
	int cy = abs(m_y - y);
	
	
	if ((cx != 1 && cy != 1 ))
	{	
		//走了好多格....
		
		if(cx>1)
		{
			//MessageBox(NULL, "不让你横着走..", "", MB_OK);
			return PM_UNREACHABLE;
		}
		
		//这是要跨棋盘吃对方老将呢.~
		piece = GetPiece(x, y);
		if(piece == NULL || piece->GetTagID() != TAG_KING || piece->GetCamp() == this->GetCamp())
		{
			//要干嘛要干嘛?
			return PM_UNREACHABLE;
		}
		
		min = y;
		max = y;
		flag = false;
		if (y > m_y)
		{
			min = m_y;
		}else{
			max = m_y;
		}
		for(i=min+1; i<max; i++)
		{
			if (GetPiece(x, i) != NULL)
			{
				flag = true;
				break;
			}
		}
		if(!flag)
		{
			return PM_CAPTURE;
		}
		
		//MessageBox(NULL, "因为有子不让你吃!", "", MB_OK);
		return PM_UNREACHABLE;
		
	}

	
	if (		(m_y<3 && y>=3)
		||	(m_y>6 && y<=6)
		||	(x < 3)
		||	(x > 5)
	)
	{
		//如果不想飞越棋盘就不许走到方格外面神马的...
		return PM_UNREACHABLE;
	}
	
	
	if (y == m_y)
	{
		//找对方主帅..
		if (y<3)
		{
			ey = 7;
		}else{
			ey = 0;
		}
		piece = NULL;
		for(i=0; i<3; i++)
		{
			p1 = m_board->GetPiece(x, ey);
			if(p1 != NULL)
			{
				if(p1->GetTagID() == TAG_KING && p1->GetCamp()!=this->GetCamp())
				{
					piece = p1;
					break;
				}
			}
			ey ++;
		}
		
		flag = false;
		if (piece != NULL)
		{
			//果然在这条线上....
			min = ey;
			max = ey;
			if (y > ey)
			{
				max = y;
			}else{
				min = y;
			}
			for(i=min+1; i<max; i++)
			{
				if (GetPiece(x, i) != NULL)
				{
					flag = true;
					break;
				}
			}
			if(!flag)
			{
				return PM_UNREACHABLE;
			}
		}
		
	}
	
	
	//总算可以走了..
	
	piece = m_board->GetPiece(x, y);
	
	if(piece == NULL)
	{
		//空地...
		return PM_MOVE;
	}
	
	if(piece->GetCamp() == this->GetCamp())
	{
		//自己人...
		return PM_UNREACHABLE;
	}
	
	//敌人...
	return PM_CAPTURE;
	
}





















