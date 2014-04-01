
#include "gnufuncs.h"

#ifdef  _MSC_VER

#include <string.h>
#include <ctype.h>

#define snprintf _snprintf

int strcasecmp(const char *s1, char *s2) 
{
        while (toupper((unsigned char)*s1) == toupper((unsigned char)*s2++)) 
                if (*s1++ == '\0')  
                        return 0;
        return(toupper((unsigned char)*s1) - toupper((unsigned char)*--s2));
}

#endif
