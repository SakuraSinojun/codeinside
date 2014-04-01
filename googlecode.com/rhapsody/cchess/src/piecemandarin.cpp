
/*
 *  PieceMandarin.cpp
 *
 *  Mandarin Piece Solution
 *
 *  Copyright (C) 2010	SakuraSinojun@gmail.com
 *
 *  2010-11-12 Created.
 *
 *  Description: This file includes the moving-rule about Mandarin pieces. 
 *
 */


#include "libcchess.h"

#include "math.h"


CPieceMandarin::CPieceMandarin(CChessBoard * board)
{
	m_board = board;
	m_tagid = TAG_MANDARIN;
	board->AddPiece(this);
	m_x = 3;
	m_y = 0;
}

CPieceMandarin::~CPieceMandarin()
{
}

int CPieceMandarin::MoveTest(int x, int y)
{
	if(!CheckPoint(x, y))
	{
		return PM_UNREACHABLE;
	}
	
	if(m_x == x && m_y == y)
	{
		return PM_UNREACHABLE;
	}
	
	if (		(m_y<3 && y>=3)
		||	(m_y>6 && y<=6)
		||	(x < 3)
		||	(x > 5)
	)
	{
		//不许走到方格外面神马的...
		return PM_UNREACHABLE;
	}
	
	int cx = abs(m_x - x);
	int cy = abs(m_y - y);
	
	if ((cx == 1 && cy == 1 ))
	{	
		//士走斜....
	}else{
		return PM_UNREACHABLE;
	}
	
	CPiece * piece = m_board->GetPiece(x, y);
	
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





















