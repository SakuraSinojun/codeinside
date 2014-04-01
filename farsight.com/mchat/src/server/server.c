

#include "mcafx.h"

#include "server.h"
#include "types.h"
#include "controller.h"
#include "network.h"
#include "ptc.h"

#include <string.h>
#include <stdio.h>
#include <stdlib.h>


client_sock     sock[FD_SETSIZE];

bool            is_running;


///////////////////////////////////////////////////////////////////////////////
//  
//     本模块函数
//

static void init_sock(int index)
{
        memset(&sock[index], 0, sizeof(client_sock));
        sock[index].sock = INVALID_SOCKET;
        sock[index].send_index = -1;
        sock[index].last_act_time = (time_t)0;
        sock[index].error_pack_count = 0;
        sock[index].state = 0;
}


static void print_time(FILE * fp)
{
        time_t                  time_now;
        struct tm *             ptm;

        time_now = time(NULL);
        ptm = localtime(&time_now);

        fprintf(fp, "%04d-%02d-%02d %02d:%02d:%02d\t", 
                ptm->tm_year+1900, ptm->tm_mon+1, ptm->tm_mday,
                ptm->tm_hour, ptm->tm_min, ptm->tm_sec);
}

static void _calc_heart()
{
        time_t          time_now;
        int             i;

        time_now = time(NULL);
        for(i=1; i<FD_SETSIZE; i++)
        {
                if(sock[i].sock != INVALID_SOCKET)
                {
                        if( (time_now - sock[i].last_act_time) > (time_t)(TIMEOUT_SECOND) )
                        {
                                closesocket(sock[i].sock);
                                init_sock(i);
                        }
                }
        }
}

void acpt_poll(fd_set * fdsr)
{
        int                     i;
        SOCKET                  socktemp;
        struct sockaddr_in      addr;
        socklen_t               len;


        if( !(FD_ISSET(sock[0].sock, fdsr)) )
        {
                return;
        }

        // 数组中找个空的位置。留三个位置给stdin stdout stderror. 
        for(i=1; i<FD_SETSIZE-3; i++)
        {
                if(sock[i].sock == INVALID_SOCKET)
                {
                        break;
                }
        }
        
        // 数组满了。这里如果不关闭会造成客户端持续等待。
        if(i >= FD_SETSIZE-3)
        {
                len = sizeof(socktemp);
                socktemp = accept(sock[0].sock, (struct sockaddr *)&addr, &len);
                if(socktemp == INVALID_SOCKET)
                {
                        pnerror("accept");
                        return;
                }
                closesocket(socktemp);
                print_time(stdout);
                printf("%s is trying to connect, but the connection queue is full.\n>", inet_ntoa(addr.sin_addr));
                fflush(stdout);
                return;
        }

        // 未满。
        len = sizeof(struct sockaddr_in);
        sock[i].sock = accept(sock[0].sock, (struct sockaddr *)&sock[i].addr, &len);
        if(sock[i].sock == INVALID_SOCKET)
        {
                pnerror("accept");
                init_sock(i);
                return;
        }
        print_time(stdout);
        printf("%s connected at %d.\n>", inet_ntoa(sock[i].addr.sin_addr), i);
        fflush(stdout);
        sock[i].last_act_time = time(NULL);

}

void recv_poll(fd_set * fdsr)
{
        int             i;
        ssize_t         bytes_read;
        char            buffer[sizeof(MCPACK)];

        // i=0是listen socket.
        for(i=1; i<FD_SETSIZE; i++)
        {
                if(sock[i].sock != INVALID_SOCKET)
                {
                        if( !FD_ISSET(sock[i].sock, fdsr) )
                        {
                                continue;
                        }

                        memset(buffer, 0, sizeof(MCPACK));
                        bytes_read = recv(sock[i].sock, buffer, sizeof(MCPACK), 0);
                        
                        if(bytes_read <= 0)
                        {
                                print_time(stdout);
                                printf("%s closed.\n>", inet_ntoa(sock[i].addr.sin_addr));
                                fflush(stdout);
                                closesocket(sock[i].sock);
                                init_sock(i);
                                continue;
                        }
                        if( !ptc_parse(i, buffer, bytes_read) )
                        {
                                print_time(stdout);
                                printf("%s closed.\n>", inet_ntoa(sock[i].addr.sin_addr));
                                fflush(stdout);
                                closesocket(sock[i].sock);
                                init_sock(i);
                        }
                        sock[i].last_act_time = time(NULL);
                }
        }
}

void send_poll(fd_set * fdsw)
{
        // TODO
}


///////////////////////////////////////////////////////////////////////////////
//  
//     导出函数
//

void ser_Send(int index, void * data, int len)
{
        if(sock[index].sock == INVALID_SOCKET)
        {
                return;
        }

        if( send(sock[index].sock, data, len, 0) <= 0 )
        {
                closesocket(sock[index].sock);
                init_sock(index);
        }

}

void ser_Kick(int index)
{
}

int ser_Start()
{
        unsigned short int      port;
        struct sockaddr_in      addr;
        fd_set                  fdsr;
        struct timeval          tv;
        int                     i;
        SOCKET                  fd_max;
        int                     ret;

        //各种初始化

        if(!network_init())
        {
                pnerror("init");
                return -1;
        }
        
        for(i=0; i<FD_SETSIZE; i++)
        {
                init_sock(i);
        }

        sock[0].sock = socket(PF_INET, SOCK_STREAM, 0);
        if(sock[0].sock == INVALID_SOCKET)
        {
                pnerror("socket");
                return -1;
        }
        
        memset(&addr, 0, sizeof(addr));
        addr.sin_family = AF_INET;
        addr.sin_addr.s_addr = htonl(INADDR_ANY);
        
        for(port = 10234; port < 10240; port++)
        {
                addr.sin_port = htons(port);
                print_time(stdout);
                printf("trying to bind to port: %5d ", port);
                if( bind(sock[0].sock, (struct sockaddr *)&addr, sizeof(addr)) == 0 )
                {
                        printf("\t [  OK  ]\n");
                        break;
                }
                printf("\t [ Fail ]\n");
        }

        if(port == 10240)
        {
                pnerror("bind");
                return -1;
        }

        if( listen(sock[0].sock, 5) != 0 )
        {
                pnerror("listen");
                return -1;
        }
        
        is_running = true;
        
        while(is_running)
        {
                FD_ZERO(&fdsr);

                tv.tv_sec = SELECT_TIMEOUT;
                tv.tv_usec = 0;

                fd_max = 0;
                for(i=0; i<FD_SETSIZE; i++)
                {
                        if(sock[i].sock != INVALID_SOCKET)
                        {
                                FD_SET(sock[i].sock, &fdsr);
                                if(sock[i].sock > fd_max)
                                {
                                        fd_max = sock[i].sock;
                                }
                        }
                }
                
                // demo: 不选fdsw，如果速度比较慢再重新设计。
                ret = select(fd_max+1, &fdsr, NULL, NULL, &tv);
                switch (ret)
                {
                case -1:
                        pnerror("select");
                        if(pnerrno() == EINTR)
                        {
                                continue;
                        }
                        for(i=0; i<FD_SETSIZE; i++)
                        {
                                closesocket(sock[i].sock);
                        }
                        return -1;
                        break;
                case 0:
                        // 超时检测心跳包。
                        _calc_heart();
                        break;
                default:
                        acpt_poll(&fdsr);
                        recv_poll(&fdsr);
                        _calc_heart();
                        break;
                }

        }


        return 0;

}

