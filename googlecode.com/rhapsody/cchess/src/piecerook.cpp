
/*
 *  piecerook.cpp
 *
 *  Rook Piece Solution
 *
 *  Copyright (C) 2010	SakuraSinojun@gmail.com
 *
 *  2010-11-12 Created.
 *
 *  Description: This file includes the moving-rule about Rook pieces. 
 *
 */


#include "libcchess.h"


CPieceRook::CPieceRook(CChessBoard * board)
{
	m_board = board;
	m_tagid = TAG_ROOK;
	board->AddPiece(this);
	m_x = 0;
	m_y = 0;
}

CPieceRook::~CPieceRook()
{
}

int CPieceRook::MoveTest(int x, int y)
{
	if(!CheckPoint(x, y))
	{
		return PM_UNREACHABLE;
	}
	
	if(m_x != x && m_y != y)
	{
		return PM_UNREACHABLE;
	}
	
	if(m_x == x && m_y == y)
	{
		return PM_UNREACHABLE;
	}
	
	int i;
	int min, max;
	CPiece * piece;
	
	//远距离障碍赛...
	if(m_x != x)
	{
		//横向移动
		min = x;
		max = x;
		
		if(x < m_x)
		{
			max = m_x;
		}else{
			min = m_x;
		}
		
		for(i=min+1; i<max; i++)
		{
			piece = m_board->GetPiece(i, y);
			if (piece != NULL)
			{
				return PM_UNREACHABLE;
			}
		}
	}else{	
		//纵向移动
		min = y;
		max = y;
		
		if(y < m_y)
		{
			max = m_y;
		}else{
			min = m_y;
		}
		
		for(i=min+1; i<max; i++)
		{
			piece = m_board->GetPiece(x, i);
			if (piece != NULL)
			{
				return PM_UNREACHABLE;
			}
		}
	}
	
	//无障碍..
	piece = m_board->GetPiece(x, y);
	if(piece == NULL)
	{
		return PM_MOVE;
	}
	if(piece->GetCamp() == this->GetCamp())
	{
		return PM_UNREACHABLE;
	}
	return PM_CAPTURE;

}


