


#pragma once

#ifndef __PRELOAD_H__
#define __PRELOAD_H__

#include <windows.h>

namespace pl
{
	HDC Load(LPCSTR filename,BITMAP * bitmap=NULL);
	void UnLoad(LPCSTR filename);

};

#endif


