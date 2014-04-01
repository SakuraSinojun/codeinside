

#include "chinesechess.h"
#include "libcchess.h"
#include <vector>
using namespace std;

CChineseChess::CChineseChess()
{
	m_lastSelectedPiece = NULL;
}

CChineseChess::~CChineseChess()
{
	ClearChessBoard();
}

	

int CChineseChess::InitChessBoard(bool usingRed)
{

	int 			campUp,	campDown;
	CHESSBOARD 		chbd;
	int 			i,	j;
	
	
	
	if(usingRed)
	{
		campUp = CAMP_BLACK;
		campDown = CAMP_RED;
	}else{
		campUp = CAMP_RED;
		campDown = CAMP_BLACK;
	}
	
	
	memset(&chbd, 0, sizeof(chbd));
	
	
	for (j=0; j<5; j++)
	{
		for(i=0; i<9; i++)
		{
			chbd.sqr[i][j].Camp = campUp;
			chbd.sqr[i][j+5].Camp = campDown;
			chbd.sqr[i][j].Direction = 1;
			chbd.sqr[i][j+5].Direction = -1;
		}
	}
	//棋盘上方:
	//左车
	chbd.sqr[0][0].hasPiece = true;
	chbd.sqr[0][0].PieceTag = TAG_ROOK;
	//左马
	chbd.sqr[1][0].hasPiece = true;
	chbd.sqr[1][0].PieceTag = TAG_KNIGHT;
	//左相
	chbd.sqr[2][0].hasPiece = true;
	chbd.sqr[2][0].PieceTag = TAG_BISHOP;
	//左士
	chbd.sqr[3][0].hasPiece = true;
	chbd.sqr[3][0].PieceTag = TAG_MANDARIN;
	//上将
	chbd.sqr[4][0].hasPiece = true;
	chbd.sqr[4][0].PieceTag = TAG_KING;
	//右士
	chbd.sqr[5][0].hasPiece = true;
	chbd.sqr[5][0].PieceTag = TAG_MANDARIN;
	//右相
	chbd.sqr[6][0].hasPiece = true;
	chbd.sqr[6][0].PieceTag = TAG_BISHOP;
	//右马
	chbd.sqr[7][0].hasPiece = true;
	chbd.sqr[7][0].PieceTag = TAG_KNIGHT;
	//右车
	chbd.sqr[8][0].hasPiece = true;
	chbd.sqr[8][0].PieceTag = TAG_ROOK;
	//左炮
	chbd.sqr[1][2].hasPiece = true;
	chbd.sqr[1][2].PieceTag = TAG_CANNON;
	//右炮
	chbd.sqr[7][2].hasPiece = true;
	chbd.sqr[7][2].PieceTag = TAG_CANNON;
	//卒1
	chbd.sqr[0][3].hasPiece = true;
	chbd.sqr[0][3].PieceTag = TAG_PAWN;
	//卒2
	chbd.sqr[2][3].hasPiece = true;
	chbd.sqr[2][3].PieceTag = TAG_PAWN;
	//卒3
	chbd.sqr[4][3].hasPiece = true;
	chbd.sqr[4][3].PieceTag = TAG_PAWN;
	//卒4
	chbd.sqr[6][3].hasPiece = true;
	chbd.sqr[6][3].PieceTag = TAG_PAWN;
	//卒5
	chbd.sqr[8][3].hasPiece = true;
	chbd.sqr[8][3].PieceTag = TAG_PAWN;

	//棋盘下方:
	//左车
	chbd.sqr[0][9].hasPiece = true;
	chbd.sqr[0][9].PieceTag = TAG_ROOK;
	//左马
	chbd.sqr[1][9].hasPiece = true;
	chbd.sqr[1][9].PieceTag = TAG_KNIGHT;
	//左相
	chbd.sqr[2][9].hasPiece = true;
	chbd.sqr[2][9].PieceTag = TAG_BISHOP;
	//左士
	chbd.sqr[3][9].hasPiece = true;
	chbd.sqr[3][9].PieceTag = TAG_MANDARIN;
	//上将
	chbd.sqr[4][9].hasPiece = true;
	chbd.sqr[4][9].PieceTag = TAG_KING;
	//右士
	chbd.sqr[5][9].hasPiece = true;
	chbd.sqr[5][9].PieceTag = TAG_MANDARIN;
	//右相
	chbd.sqr[6][9].hasPiece = true;
	chbd.sqr[6][9].PieceTag = TAG_BISHOP;
	//右马
	chbd.sqr[7][9].hasPiece = true;
	chbd.sqr[7][9].PieceTag = TAG_KNIGHT;
	//右车
	chbd.sqr[8][9].hasPiece = true;
	chbd.sqr[8][9].PieceTag = TAG_ROOK;
	//左炮
	chbd.sqr[1][7].hasPiece = true;
	chbd.sqr[1][7].PieceTag = TAG_CANNON;
	//右炮
	chbd.sqr[7][7].hasPiece = true;
	chbd.sqr[7][7].PieceTag = TAG_CANNON;
	//卒1
	chbd.sqr[0][6].hasPiece = true;
	chbd.sqr[0][6].PieceTag = TAG_PAWN;
	//卒2
	chbd.sqr[2][6].hasPiece = true;
	chbd.sqr[2][6].PieceTag = TAG_PAWN;
	//卒3
	chbd.sqr[4][6].hasPiece = true;
	chbd.sqr[4][6].PieceTag = TAG_PAWN;
	//卒4
	chbd.sqr[6][6].hasPiece = true;
	chbd.sqr[6][6].PieceTag = TAG_PAWN;
	//卒5
	chbd.sqr[8][6].hasPiece = true;
	chbd.sqr[8][6].PieceTag = TAG_PAWN;
	
	
	SetChessBoard(chbd);
	
	return CC_SUCCESS;

}

int CChineseChess::StartTurn(int camp)
{
	m_turn = camp;
	return CC_SUCCESS;
}

int CChineseChess::ClearChessBoard()
{
	vector<void *>::iterator it;
	CPiece * piece;
	
	for(it=m_v.begin(); it!=m_v.end(); it++)
	{
		piece = (CPiece *)(*it);
		m_v.erase(it);
		delete piece;
	}
	m_lastSelectedPiece = NULL;
	m_board.ClearAllPiece();
	
	return CC_SUCCESS;
	
}
	
int CChineseChess::SetChessBoard(CHESSBOARD& chbd)
{
	int i, j;
	CPiece * piece;
	
	
	ClearChessBoard();
	
	for(i=0; i<9; i++)
	{
		for(j=0; j<10; j++)
		{
			if(chbd.sqr[i][j].hasPiece)
			{
				switch (chbd.sqr[i][j].PieceTag)
				{
				case TAG_ROOK:
					piece = new CPieceRook(&m_board);
					break;
				case TAG_KNIGHT:
					piece = new CPieceKnight(&m_board);
					break;
				case TAG_BISHOP:
					piece = new CPieceBishop(&m_board);
					break;
				case TAG_MANDARIN:
					piece = new CPieceMandarin(&m_board);
					break;
				case TAG_KING:
					piece = new CPieceKing(&m_board);
					break;
				case TAG_CANNON:
					piece = new CPieceCannon(&m_board);
					break;
				case TAG_PAWN:
					piece = new CPiecePawn(&m_board);
					piece->SetDirection(chbd.sqr[i][j].Direction);
					break;
				default:
					piece = NULL;
					break;
				}
				if(piece != NULL)
				{
					piece->m_x = i;
					piece->m_y = j;
					piece->SetCamp(chbd.sqr[i][j].Camp);
					m_v.push_back(piece);
				}
			}
		}
	}
	
	return CC_SUCCESS;
}

int CChineseChess::GetChessBoard(CHESSBOARD& chbd)
{
	memset(&chbd, 0, sizeof(chbd));
	int i,j;
	CPiece * piece;
	
	for(i=0; i<9; i++)
	{
		for(j=0; j<10; j++)
		{
			piece = m_board.GetPiece(i, j);
			if(piece != NULL)
			{
				chbd.sqr[i][j].hasPiece = true;
				chbd.sqr[i][j].PieceTag = piece->GetTagID();
				chbd.sqr[i][j].Camp = piece->GetCamp();
				chbd.sqr[i][j].Selected = false;
				if(piece == m_lastSelectedPiece)
				{
					chbd.sqr[i][j].Selected = true;
				}
			}
		}
	}
	
	return CC_SUCCESS;
}

	
int CChineseChess::Click(int x, int y)
{
	CPiece * piece;
	
	piece = m_board.GetPiece(x, y);
	
	if(m_lastSelectedPiece != NULL)
	{
		//选中过棋子
		
		if(piece != NULL)
		{
			if(piece->GetCamp() == m_lastSelectedPiece->GetCamp())
			{
				m_lastSelectedPiece = NULL;
				return Click(x, y);
			}
		}
		
		int res = m_lastSelectedPiece->Move(x, y);
		
		switch(res)
		{
		case PM_UNREACHABLE:
			return CL_UNREACHABLE;
			break;
		case PM_MOVE:
			m_lastSelectedPiece = NULL;
			m_turn = CAMP_OVER - m_turn;
			return CL_MOVE;
		case PM_CAPTURE:
			m_lastSelectedPiece = NULL;
			m_turn = CAMP_OVER - m_turn;
			return CL_CAPTURE;
		default:
			return CC_ERROR;
			break;
		}
		
	}else{
		//没有选择棋子

		if(piece == NULL)
		{
			return CL_NONESELECTED;
		}
	
		if(piece->GetCamp() != m_turn)
		{
			return CL_NOTYOURPIECE;
		}
		m_lastSelectedPiece = piece;
	}
	
	return CC_SUCCESS;
}
	
int CChineseChess::WhoWin()
{
	vector<CPiece *>::iterator it;
	CPiece * piece;
	
	bool redKing = false;
	bool blackKing = false;
	
	for(it=m_board.m_vpiece.begin(); it!=m_board.m_vpiece.end(); it++)
	{
		piece = *it;
		if(piece->GetTagID() == TAG_KING)
		{
			if(piece->GetCamp() == CAMP_RED)
			{
				redKing = true;
			}else if(piece->GetCamp()==CAMP_BLACK){ 
				blackKing = true;
			}
		}
	}
	
	if(redKing && blackKing)
	{
		return CW_NOBODY;
	}
	if(!redKing && !blackKing)
	{
		return CW_NOBODY;
	}
	if(redKing)
	{
		return CW_RED;
	}
	if(blackKing)
	{
		return CW_BLACK;
	}
	return CW_NOBODY;
}




















