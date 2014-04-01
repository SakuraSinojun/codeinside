
#include "config.h"
#include <stdio.h>
#include <stdlib.h>

#include "dump.h"

namespace tools {

void dump(const void * data, int len)
{
    const char *    p = (const char *)(data);
    int             i;
    int             j;
    int             t;
    int             k;

    for(i=0; i<len; i+=16)
    {
        printf("%08x: ", i);
        t = 16;
        if(i + 16 > len)
            t = len - i;
        if(t == 16)
        {
            for(k=i; k<len; k++)
            {
                if(p[k] != '\0')
                    break;
            }
            if(k - i > 16 * 3)
            {
                k = k - (( k - i ) % 16);
                printf(" 00 * %d lines\n", (k -i) / 16);
                i = k - 16; 
                continue;
            }
        }
        for(j=0; j<t; j++)
        {
            printf("%02x ", p[i+j] & 0xff);
            if((j % 4) == 3)
                printf(" ");
        }
        for(j=t; j<16; j++)
        {
            printf("   ");
            if((j % 4) == 3)
                printf(" ");
        }
        printf("\t");
        for(j=0; j<t; j++)
        {
            if((p[i+j] >= 36 && p[i+j] <= 126) || p[i+j] == '"')
                printf("%c", p[i+j] & 0xff);
            else
                printf(".");
        }
        printf("\n");
    }
}

} // namespace tools




