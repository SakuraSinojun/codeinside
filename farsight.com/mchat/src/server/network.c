

#include "network.h"

const char * network_error()
{
        char * buffer;
        
#ifndef _WIN32
        buffer = strerror(errno);
#else
        FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_ALLOCATE_BUFFER, 
                      NULL, 
                      WSAGetLastError(), 
                      MAKELANGID(LANG_ENGLISH, SUBLANG_ENGLISH_US), 
                      (PSTR)&buffer, 0, NULL);
#endif
        return buffer;
    
}

int network_init()
{
#ifdef _WIN32

        WSADATA wsaData;
        WORD    version = MAKEWORD(2, 0);
        int     ret = WSAStartup(version, &wsaData);
        if(ret)
        {
                network_error("WSAStartup");
                return 0;
        }
#endif
        return 1;
}



void pnerror(const char * tips)
{
         printf("%s: %s\n", tips, network_error());
}

int pnerrno()
{
#ifdef _WIN32
        return WSAGetLastError();
#else
        return errno;
#endif

}

