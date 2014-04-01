
/*
 *  PieceKnight.cpp
 *
 *  Knight Piece Solution
 *
 *  Copyright (C) 2010	SakuraSinojun@gmail.com
 *
 *  2010-11-12 Created.
 *
 *  Description: This file includes the moving-rule about Knight pieces. 
 *
 */


#include "libcchess.h"

#include "math.h"


CPieceKnight::CPieceKnight(CChessBoard * board)
{
	m_board = board;
	m_tagid = TAG_KNIGHT;
	board->AddPiece(this);
	m_x = 1;
	m_y = 0;
}

CPieceKnight::~CPieceKnight()
{
}

int CPieceKnight::MoveTest(int x, int y)
{
	if(!CheckPoint(x, y))
	{
		return PM_UNREACHABLE;
	}
	
	if(m_x == x && m_y == y)
	{
		return PM_UNREACHABLE;
	}
	
	int cx = abs(m_x - x);
	int cy = abs(m_y - y);
	
	if ( 		(cx == 1 && cy == 2 )
		||	(cx == 2 && cy == 1 )
	)
	{	
		//马走日....
	}else{
		return PM_UNREACHABLE;
	}
	
	CPiece * piece = NULL;
	if (cx == 2)
	{
		//横向别马腿...
		piece = m_board->GetPiece(m_x + (x - m_x)/2, m_y);
	}else{
		//纵向别马腿......
		piece = m_board->GetPiece(m_x, m_y + (y - m_y)/2 );
	}
	
	if(piece != NULL)
	{
		//果然别马腿...
		return PM_UNREACHABLE;
	}
	
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





















