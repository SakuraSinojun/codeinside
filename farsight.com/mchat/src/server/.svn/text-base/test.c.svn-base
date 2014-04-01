

#include "mcafx.h"
#include "network.h"

#include <string.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char * argv[])
{

        SOCKET                  sock;
        struct sockaddr_in      addr;
        MCPACK                  pack;
        ssize_t                 bytes_read;
        char                    arg[1024];
        USERINFO                self;
        int                     i;

        
        if(argc == 1)
        {
                strcpy(arg, "rlafci");
        }else{
                strcpy(arg, argv[1]);
        }
        
        if(! network_init() )
        {
                pnerror("init");
                return 0;
        }

        sock = socket(PF_INET, SOCK_STREAM, 0);
        if(sock == INVALID_SOCKET)
        {
                pnerror("socket");
                return 0;
        }

        addr.sin_family = AF_INET;
        addr.sin_addr.s_addr = inet_addr("127.0.0.1");
        addr.sin_port = htons(10234);
        
        if( connect(sock, (struct sockaddr *)&addr, sizeof(addr)) != 0 )
        {
                pnerror("connect");
                return 0;
        }


        // register
        if(strchr(arg, 'r') != NULL)
        {
                pack.action = MA_REGISTER_REQUEST;
                strcpy(pack.regist.username, "Sakura");
                strcpy(pack.regist.password, "Ioriesy");

                send(sock, (const char *)&pack, sizeof(pack), 0);
                bytes_read = recv(sock, (char *)&pack, sizeof(pack), 0);
                printf("action=%d, uid=%d, state=%d, message=%s\n", pack.action, pack.answer.uid, pack.answer.state, pack.answer.message);
                closesocket(sock);
                return 0;
        }

	self.uid = 0;

        // login
        if(strchr(arg, 'l') != NULL)
        {
                pack.action = MA_LOGIN_REQUEST;
                strcpy(pack.regist.username, "Sakura");
                strcpy(pack.regist.password, "Ioriesy");

                send(sock, (const char *)&pack, sizeof(pack), 0);
                bytes_read = recv(sock, (char *)&pack, sizeof(pack), 0);
                printf("action=%d, uid=%d, state=%d, message=%s\n", pack.action, pack.answer.uid, pack.answer.state, pack.answer.message);
                
                self.uid = pack.answer.uid;
        }

        // add friend
        if(strchr(arg, 'a') != NULL)
        {
                pack.action = MA_ADD_FRIEND;
                pack.request.uid = self.uid;
                send(sock, (const char *)&pack, sizeof(pack), 0);
                bytes_read = recv(sock, (char *)&pack, sizeof(pack), 0);
                printf("action=%d, uid=%d\n", pack.action, pack.request.uid);
                
                pack.action = MA_ADD_FRIEND_ANSWER;
                //pack.request.uid = self.uid;
                pack.answer.state = 0;
                pack.answer.uid = self.uid;
                strcpy(pack.answer.message, "heya");
                send(sock, (const char *)&pack, sizeof(pack), 0);
                bytes_read = recv(sock, (char *)&pack, sizeof(pack), 0);
                printf("action=%d, uid=%d, state=%d, message=%s\n", pack.action, pack.answer.uid, pack.answer.state, pack.answer.message);

        }

        // query friend list
        if(strchr(arg, 'f') != NULL)
        {
                memset(&pack, 0, sizeof(pack));
                pack.action = MA_QUERY_FRIEND_LIST;
                send(sock, (const char *)&pack, sizeof(pack), 0);
                bytes_read = recv(sock, (char *)&pack, sizeof(pack), 0);

                printf("action=%d\n", pack.action);

                for(i=0; i<MAX_FRIENDS_COUNT; i++)
                {
                        if(pack.list.uid[i] != 0)
                        {
                                printf("\t%d\n", pack.list.uid[i]);
                        }
                }

        }

        // query user info
        if(strchr(arg, 'i') != NULL)
        {
                memset(&pack, 0, sizeof(pack));
                pack.action = MA_QUERY_USER_INFO;
                pack.request.uid = self.uid;
                send(sock, (const char *)&pack, sizeof(pack), 0);
                bytes_read = recv(sock, (char *)&pack, sizeof(pack), 0);

                printf("%d state: online:%d\n", pack.userinfo.uid, pack.userinfo.onlinestate);

        }

        // chat
        if(strchr(arg, 'c') != NULL)
        {
                memset(&pack, 0, sizeof(pack));
                pack.action = MA_CHAT;
                pack.chat.uid = self.uid;
                pack.chat.gid = 0;
                strcpy(pack.chat.message, "Good Morning~");
                send(sock, (const char *)&pack, sizeof(pack), 0);
                bytes_read = recv(sock, (char *)&pack, sizeof(pack), 0);

                printf("action=%d, uid=%d, message=%s\n", pack.action, pack.chat.uid, pack.chat.message);

        }

       

        closesocket(sock);


        return 0;

}
