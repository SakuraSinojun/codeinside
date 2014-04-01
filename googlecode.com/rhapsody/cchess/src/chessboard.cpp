


/*
 *  chessboard.cpp
 *
 *  Chess Bord Definition
 *
 *  Copyright (C) 2010	SakuraSinojun@gmail.com
 *
 *  2010-11-11 Created.
 *
 *  Description: 
 *
 */

#include "chessdef.h"
#include "chessboard.h"
#include "piece.h"

#include <vector>
using namespace std;


CChessBoard::CChessBoard()
{
}

CChessBoard::~CChessBoard()
{
}

CPiece * CChessBoard::GetPiece(int x, int y)
{
	vector<CPiece *>::iterator it;
	
	
	if(x<0 || y<0 || x>8 || y>9)
	{
		return NULL;
	}
	
	
	for(it=m_vpiece.begin(); it!=m_vpiece.end(); it++)
	{
		if ((*it)->m_x == x && (*it)->m_y == y)
		{
			return (CPiece*)(*it);
		}
		
	}
	return NULL;
}

int CChessBoard::AddPiece(CPiece * piece)
{
	m_vpiece.push_back(piece);
	return BD_SUCCESS;
}

int CChessBoard::RemovePiece(CPiece * piece)
{
	vector<CPiece *>::iterator it;
	
	for(it=m_vpiece.begin(); it!=m_vpiece.end(); it++)
	{
		if ( (CPiece *)(*it) == piece )
		{
			m_vpiece.erase( it );
			return BD_SUCCESS;
		}
	}
	return BD_SUCCESS;
}

int CChessBoard::RemovePiece(int x, int y)
{
	CPiece * piece = GetPiece(x, y);
	if (piece == NULL)
	{
		return BD_NOPIECE;
	}
	
	return RemovePiece(piece);

}

int CChessBoard::ClearAllPiece()
{
	m_vpiece.clear();
	return BD_SUCCESS;
}

void CChessBoard::EnumPieces(LPFNENUMPIECECALLBACK lpfnCallBack, void * lParameter)
{
	if (lpfnCallBack == NULL)
	{
		return;
	}
	vector<CPiece *>::iterator it;
	
	for(it=m_vpiece.begin(); it!=m_vpiece.end(); it++)
	{
		if( !lpfnCallBack (*it, lParameter) )
		{
			return;
		}
	}
}






