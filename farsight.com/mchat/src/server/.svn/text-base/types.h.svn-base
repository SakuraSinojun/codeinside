

#pragma once

#ifndef __TYPES_H__
#define __TYPES_H__

#include "mcafx.h"
#include "network.h"
#include "../mcprotocal.h"
#include <time.h>


#define SELECT_TIMEOUT          3
#define TIMEOUT_SECOND          60
#define PACK_SIZE               1024
#define MAX_ERROR_PACK_COUNT    50

typedef struct client_sock
{
        SOCKET                  sock;
        struct sockaddr_in      addr;
        unsigned int            send_index;
        time_t                  last_act_time;
        int                     error_pack_count;
        USERINFO		userinfo;
        int                     state;          // 0-未登陆，1-在线，2-隐身，3-离开，etc.

}client_sock;


extern client_sock	sock[FD_SETSIZE];
extern bool             is_running;



#endif




