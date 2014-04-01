/*
 *  piece.cpp
 *
 *  Chess Piece Solution
 *
 *  Copyright (C) 2010	SakuraSinojun@gmail.com
 *
 *  2010-11-12 Created.
 *
 *  Description: This file includes the solutions about common pieces. 
 *
 */


#include "libcchess.h"

CPiece::CPiece(CChessBoard * board)
{
	m_board = board;
	board->AddPiece(this);
}

CPiece::~CPiece()
{
	m_x = -1;
	m_y = -1;
	m_direction = 0;
}

int CPiece::SetPos(int x, int y)
{
	
	if(x<0 || y<0 || x>8 || y>9)
	{
		return 0;
	}
	
	m_x = x;
	m_y = y;
	
	return 1;
}

int CPiece::GetPos(int& x, int& y)
{
	x = m_x;
	y = m_y;
	return 1;
}

int CPiece::SetCamp(int camp)
{
	m_camp = camp; 
	return 0;
}

int CPiece::GetCamp()
{
	return m_camp;
}

int CPiece::GetTagID()
{
	return m_tagid;
}


bool CPiece::CheckPoint(int x, int y)
{
	if(x<0 || y<0 || x>8 || y>9)
	{
		return false;
	}
	return true;
}

int CPiece::Move(int x, int y)
{
	int res = MoveTest(x, y);
	if(res != PM_UNREACHABLE)
	{
		m_board->RemovePiece(x, y);
		m_x = x;
		m_y = y;
	}
	return res;
}

CPiece * CPiece::GetPiece(int x, int y)
{
	return m_board->GetPiece(x, y);
}


int CPiece::SetDirection(int d)
{
	if(d != 1)
	{
		d = -1;
	}
	int od = m_direction;
	m_direction = d;
	return od;
}

















