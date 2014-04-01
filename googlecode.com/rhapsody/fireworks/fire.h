
/*
 *  File:		fire.h
 *
 *  Author:		SakuraSinojun@gmail.com
 *
 *  Date:		2010-11-19
 *
 *  Tab Width:	6
 *
 */
 

#pragma once

#ifndef __FIRE_H__
#define __FIRE_H__


#include <vector>
#include <windows.h>

struct FireStar
{
	double	direction;
	double	vx;
	double	vy;
	double	ox;
	double	oy;
	double	x;
	double	y;
	double	sy;
	double	step;
	int		radius;
	int		r;
	int		g;
	int		b;
	int 		orr;
	int		ogg;
	int		obb;
	LONGLONG 	starttime;
	double	alivetime;
	int		FW_STEP;
};

typedef bool (* LPFNCALLBACK)(FireStar * piece, void * lParameter);

class CFire
{

public:
	CFire();
	~CFire();
	CFire(int x, int y);
	
public:
	void Init(int x, int y);
	
public:
	bool NextFrame();
	bool CalcFrame();
	
	void EnumStars(LPFNCALLBACK lpfnCallBack, void * lParameter);
	FireStar * GetFirstStar();
	FireStar * GetNextStar();
	bool IsStopped();
	
private:
	bool m_running;
	std::vector<FireStar> m_v;
	std::vector<FireStar>::iterator m_it;
	
};

#endif















