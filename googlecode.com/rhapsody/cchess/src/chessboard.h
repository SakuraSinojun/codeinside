

/*
 *  chessboard.h
 *
 *  Chess Bord Definition
 *
 *  Copyright (C) 2010	SakuraSinojun@gmail.com
 *
 *  2010-11-11 Created.
 *
 *  Description: This file mainly includes the definition about chessboard. 
 *
 */



#pragma once

#ifndef __CHESSBOARD_H__
#define __CHESSBOARD_H__
	
		
	#include <vector>
	class CPiece;

	typedef bool (* LPFNENUMPIECECALLBACK)(CPiece * piece, void * lParameter);

	class CChessBoard
	{
	public:
		CChessBoard();
		~CChessBoard();
		
	public:
		CPiece * GetPiece(int x, int y);
		int AddPiece(CPiece * piece);
		int RemovePiece(CPiece * piece);
		int RemovePiece(int x, int y);
		void EnumPieces(LPFNENUMPIECECALLBACK lpfnCallBack, void * lParameter);
		int ClearAllPiece();
	public:
		std::vector<CPiece *> m_vpiece;

	};

#endif

