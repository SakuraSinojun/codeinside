/*
 *  piece.h
 *
 *  Chess Piece Definition
 *
 *  Copyright (C) 2010	SakuraSinojun@gmail.com
 *
 *  2010-11-12 Created.
 *
 *  Description: This file mainly includes the definition of ALL pieces. 
 *
 */



#pragma once

#ifndef __PIECE_H__
#define __PIECE_H__
		
	
	class CChessBoard;
	class CPiece
	{

	public:
		CPiece(){};
		CPiece(CChessBoard * board);
		virtual ~CPiece();
		
	public:
		
		int SetPos(int x, int y);
		int GetPos(int& x, int& y);
		int SetCamp(int camp);
		int GetTagID();
		int GetCamp();
		int Move(int x, int y);
		CPiece * GetPiece(int x, int y);
		
		virtual int MoveTest(int x, int y) = 0;
		
	public:
		bool CheckPoint(int x, int y);
		
		int SetDirection(int d);
	
	public:
		int m_x;
		int m_y;
	public:
		CChessBoard * m_board;
		int m_tagid;
		int m_camp;
		int m_direction;
	};
	
	
	//rook -- Ü‡
	class CPieceRook : public CPiece
	{
	public:
		CPieceRook(CChessBoard * board);
		~CPieceRook();
		
	public:
		int MoveTest(int x, int y);
		
	};
	
	//knight -- ñR
	class CPieceKnight : public CPiece
	{
	public:
		CPieceKnight(CChessBoard * board);
		~CPieceKnight();
		
	public:
		int MoveTest(int x, int y);
		
	};
	
	//bishop -- Ïà
	class CPieceBishop : public CPiece
	{
	public:
		CPieceBishop(CChessBoard * board);
		~CPieceBishop();
		
	public:
		int MoveTest(int x, int y);
		
	};
	
	//mandarin -- Ê¿
	class CPieceMandarin : public CPiece
	{
	public:
		CPieceMandarin(CChessBoard * board);
		~CPieceMandarin();
		
	public:
		int MoveTest(int x, int y);
		
	};
	
	//mandarin -- Ž›
	class CPieceKing : public CPiece
	{
	public:
		CPieceKing(CChessBoard * board);
		~CPieceKing();
		
	public:
		int MoveTest(int x, int y);
		
	};
	
	//mandarin -- ÅÚ
	class CPieceCannon : public CPiece
	{
	public:
		CPieceCannon(CChessBoard * board);
		~CPieceCannon();
		
	public:
		int MoveTest(int x, int y);
		
	};
	
	//mandarin -- ±ø
	class CPiecePawn : public CPiece
	{
	public:
		CPiecePawn(CChessBoard * board);
		~CPiecePawn();
	
	public:
		int MoveTest(int x, int y);

	};
	
#endif































