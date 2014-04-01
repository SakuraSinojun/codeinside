
/*
 *  chessdef.h
 *
 *  Const Definition
 *
 *  Copyright (C) 2010	SakuraSinojun@gmail.com
 *
 *  2010-11-12 Created.
 *
 *  Description: This file includes the const values. 
 *
 */


#pragma once

#ifndef __CHESSDEF_H__
#define __CHESSDEF_H__


//Piece Move
#define	PM_MOVE			0
#define	PM_UNREACHABLE		1
#define	PM_CAPTURE			2


//Board
#define	BD_SUCCESS			0 	
#define 	BD_NOPIECE			1

//TagID
enum TAGID
{
	TAG_ROOK,
	TAG_KNIGHT,
	TAG_BISHOP,
	TAG_MANDARIN,
	TAG_KING,
	TAG_CANNON,
	TAG_PAWN
};


#define	CAMP_RED		0 
#define	CAMP_BLACK		1
#define	CAMP_OVER		(CAMP_RED + CAMP_BLACK)

#endif























