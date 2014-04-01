
/*
 *  PieceBishop.cpp
 *
 *  Bishop Piece Solution
 *
 *  Copyright (C) 2010	SakuraSinojun@gmail.com
 *
 *  2010-11-12 Created.
 *
 *  Description: This file includes the moving-rule about Bishop pieces. 
 *
 */


#include "libcchess.h"

#include "math.h"


CPieceBishop::CPieceBishop(CChessBoard * board)
{
	m_board = board;
	m_tagid = TAG_BISHOP;
	board->AddPiece(this);
	m_x = 2;
	m_y = 0;
}

CPieceBishop::~CPieceBishop()
{
}

int CPieceBishop::MoveTest(int x, int y)
{
	if(!CheckPoint(x, y))
	{
		return PM_UNREACHABLE;
	}
	
	if(m_x == x && m_y == y)
	{
		return PM_UNREACHABLE;
	}
	
	if (		(m_y<5 && y>=5)
		||	(m_y>4 && y<=4)
	)
	{
		//不许过河神马的...
		return PM_UNREACHABLE;
	}
	
	int cx = abs(m_x - x);
	int cy = abs(m_y - y);
	
	if ((cx == 2 && cy == 2 ))
	{	
		//象走田....
	}else{
		return PM_UNREACHABLE;
	}
	
	CPiece * piece = NULL;
	
	//表示别象眼...
	piece = m_board->GetPiece(m_x+(x-m_x)/2, m_y+(y-m_y)/2);
	
	if(piece != NULL)
	{
		//果然别象眼...
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





















