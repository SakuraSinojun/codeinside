

#pragma once

#ifndef __DEBUG_H__
#define __DEBUG_H__

#define _CRT_SECURE_NO_WARNINGS

#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include <malloc.h>


#ifdef  __GAME_DEBUG_MESSAGE__

#ifndef __func__
#       ifdef __FUNCTION__
#               define __func__  __FUNCTION__
#       else
#               define __func__  __FILE__
#       endif
#endif


#define malloc(x) malloc_debug(x, __FILE__, __LINE__, __func__)
#define free(x) free_debug(x)

#ifdef __cplusplus
extern "C" {
#endif

void __out_debug_(const char * string, ...);
void * malloc_debug(    unsigned int num_bytes,
                        const char * file,
                        const int line,
                        const char * function
                   );


void free_debug(void * ptr);

#ifdef __cplusplus
}
#endif

#else

#ifdef __cplusplus
extern "C" {
#endif

void __out_debug_(const char * string, ...);
        
#ifdef __cplusplus
}
#endif

        
#endif

#endif

