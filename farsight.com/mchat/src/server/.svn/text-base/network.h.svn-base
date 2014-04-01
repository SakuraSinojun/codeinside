

#pragma once
#ifndef __NETWORK_H__
#define __NETWORK_H__


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#ifdef _WIN32
#include <winsock2.h>
#include <windows.h>
#include <ws2tcpip.h>
#define socklen_t       int	
#define ssize_t         int
#define sleep(x)	Sleep(x * 1000)
#else
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/wait.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <linux/un.h>
#define SOCKET          int
#define INVALID_SOCKET  (SOCKET)(-1)
#define closesocket(x)  close(x)
#endif



const char *    network_error();
void            pnerror(const char * tips);
int             pnerrno();

int             network_init();


#endif

