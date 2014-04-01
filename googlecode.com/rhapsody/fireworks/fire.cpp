
/*
 *  File:		fire.cpp
 *
 *  Author:		SakuraSinojun@gmail.com
 *
 *  Date:		2010-11-19
 *
 *  Tab Width:	6
 *
 */
 

#include "fire.h"

#include <time.h>
#include <math.h>
#include <windows.h>

CFire::CFire()
{
	m_running = false;
}

CFire::CFire(int x, int y)
{
	Init(x, y);
	m_running = true;
}

CFire::~CFire()
{
}

void CFire::Init(int x, int y)
{
	int			count;
	int			i;
	FireStar		fs;
	int			rr, gg, bb;
	LARGE_INTEGER 	litmp;

	m_v.clear();

	QueryPerformanceCounter(&litmp);	
	srand( (unsigned)litmp.QuadPart );
	
	rr = 64 + rand() % 196;
	gg = 64 + rand() % 196;
	bb = 64 + rand() % 196;
		
		
	count = 1000;								//1000个星星
	for(i=0; i<count; i++)
	{
		fs.radius = 100 + rand() % 50;				//半径
		fs.direction = (double)(rand() % 628) / 100.0f;	//角度
		fs.ox = x;
		fs.oy = y;
		fs.sy = 480;							//这个值应该是屏幕高度..
		fs.x = x;
		fs.y = fs.sy;
		fs.step = 0.3 + (double)(rand() % 100 + 1) / 100.0f;
		fs.vx = fs.step * cos(fs.direction);
		fs.vy = fs.step * sin(fs.direction);
		
		
		if (rand() % 100 > 50)
		{	
			//杂色
			fs.r = 64 + rand() % 196;
			fs.g = 64 + rand() % 196;
			fs.b = 64 + rand() % 196;
		}else{
			//主色调
			fs.r = rr;
			fs.g = gg;
			fs.b = bb;
		}
		
		fs.orr = fs.r;
		fs.ogg = fs.g;
		fs.obb = fs.b;
		
		fs.FW_STEP = 2;
		
		fs.alivetime = 3.00;	//单位是秒
		
		m_v.push_back(fs);
	}
	m_it = m_v.begin();
	m_running = true;
}

bool CFire::NextFrame()
{
	if(!m_running)
	{
		return false;
	}
	
	return false;
	
	std::vector<FireStar>::iterator	it;
	
	for (it=m_v.begin(); it!=m_v.end(); it++)
	{
		if((*it).alivetime > 0)
		{
			(*it).x += (*it).step * cos((*it).direction);
			(*it).y += (*it).step * sin((*it).direction);
		}else{
			m_running = false;
			return false;
		}
		(*it).alivetime-=0.01;
	}
	
	return true;
}

bool CFire::CalcFrame()
{
	if(!m_running)
	{
		return false;
	}
	
	
	std::vector<FireStar>::iterator	it;
	LARGE_INTEGER 				litmp;
	double					dff;
	double					tm;

	QueryPerformanceFrequency(&litmp);
	dff = (double)litmp.QuadPart;
	
	for (it=m_v.begin(); it!=m_v.end(); it++)
	{
		if((*it).FW_STEP == 2)
		{
			(*it).y -= 2;
			if((*it).y < (*it).oy)
			{
				(*it).FW_STEP = 1;
			}
		}else if ((*it).FW_STEP == 1)
		{
			QueryPerformanceCounter(&litmp);
			(*it).starttime = litmp.QuadPart;	
			(*it).FW_STEP = 0;
		}
		else
		{
		
			if (litmp.QuadPart < (*it).starttime + (*it).alivetime * dff)
			{		
				QueryPerformanceCounter(&litmp);
				tm = ( (double)(litmp.QuadPart - (*it).starttime) ) / dff;
				(*it).x = (*it).ox + (*it).step * (*it).radius * tm / (*it).alivetime * (*it).vx;
				(*it).y = (*it).oy + (*it).step * (*it).radius * tm / (*it).alivetime * (*it).vy;
				
				(*it).vy += (double)(rand() % 70) / 10000.0f;
				
				
				if( tm > (*it).alivetime/2)
				{
					(*it).r = (int)( (double)(*it).orr * ((*it).alivetime-tm) / (*it).alivetime * 2 );
					(*it).g = (int)( (double)(*it).ogg * ((*it).alivetime-tm) / (*it).alivetime * 2 );
					(*it).b = (int)( (double)(*it).obb * ((*it).alivetime-tm) / (*it).alivetime * 2 );
				}
				
				/*
				(*it).r = (int)( (*it).orr / (*it).alivetime / (*it).alivetime * ((*it).alivetime-tm) * ((*it).alivetime-tm) );
				(*it).g = (int)( (*it).ogg / (*it).alivetime / (*it).alivetime * ((*it).alivetime-tm) * ((*it).alivetime-tm) );
				(*it).b = (int)( (*it).obb / (*it).alivetime / (*it).alivetime * ((*it).alivetime-tm) * ((*it).alivetime-tm) );
				*/
			}else{
				(*it).r = 0;
				(*it).g = 0;
				(*it).b = 0;
				m_running = false;
				m_v.clear();
				
				return false;
			}
		}
	}
	return true;
	
}


void CFire::EnumStars(LPFNCALLBACK lpfnCallBack, void * lParameter)
{

}

FireStar * CFire::GetFirstStar()
{
	if(!m_running)
	{
		return NULL;
	}
	
	m_it = m_v.begin();
	
	return &(*m_it);
	
}

FireStar * CFire::GetNextStar()
{
	if(!m_running)
	{
		return NULL;
	}
	
	if (m_it == m_v.end())
	{
		return NULL;
	}
	
	m_it++;
	
	if(m_it == m_v.end())
	{
		return NULL;
	}
	
	return &(*m_it);
	
}

bool CFire::IsStopped()
{
	return !m_running;
}








