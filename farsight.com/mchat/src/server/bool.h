


#pragma once
#ifndef __BOOL_H__
#define __BOOL_H__


        #ifndef __cplusplus

                #ifdef 	__STDC__
                        #if     __STDC_VERSION__ == 199901L
                                #include <stdbool.h>
                                #define	STDC99
                        #endif
                #endif

                #ifndef	STDC99
                        #define  bool                    int
                        #define  true                    1
                        #define  false                   0
                        #define  STDC99
                #endif

        #endif

#endif

