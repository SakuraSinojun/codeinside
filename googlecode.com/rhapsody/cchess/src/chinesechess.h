

#pragma once

#ifndef __CHINESE_CHESS_H__
#define __CHINESE_CHESS_H__


#include "libcchess.h"
#include <vector>

enum CCHINESSENUM
{
	CC_SUCCESS,
	CC_ERROR,
	
	CL_NONESELECTED,
	CL_NOTYOURPIECE,
	
	CL_MOVE,
	CL_UNREACHABLE,
	CL_CAPTURE,
	
	CW_NOBODY,
	CW_RED,
	CW_BLACK,
	
};

struct PIECEINFO
{
	bool		hasPiece;
	int 		PieceTag;
	int 		Camp;
	bool		Selected;
	int		Direction;
};
struct CHESSBOARD
{
	PIECEINFO 	sqr[9][10];
};

class CChineseChess
{
public:
	CChineseChess();
	~CChineseChess();
	
public:
	int InitChessBoard(bool usingRed = true);
	int StartTurn(int camp);
	
	int SetChessBoard(CHESSBOARD& chbd);
	int GetChessBoard(CHESSBOARD& chbd);
	int ClearChessBoard();
	
	int Click(int x, int y);
	int WhoWin();
	
public:
	int m_turn;
	CChessBoard m_board;
	CPiece * m_lastSelectedPiece;
	std::vector<void *> m_v;
};

#endif




















