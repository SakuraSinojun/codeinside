
#include "mcafx.h"
#include "ptc.h"
#include "types.h"
#include "server.h"

#include <stdio.h>
#include <string.h>



/*
        MA_ZERO		= 0,
        MA_LOGIN_REQUEST,
        MA_LOGIN_ANSWER,
        MA_REGISTER_REQUEST,
        MA_REGISTER_ANSWER,
        MA_ADD_FRIEND,                  // 用户1->服务器->用户2
        MA_ADD_FRIENT_ANSWER,           // 用户2->服务器->用户1
        MA_QUERY_FRIEND_LIST,           
        MA_FRIENT_LIST,
        MA_QUERY_USER_INFO,
        MA_ACK_USER_INFO,

        MA_CREATE_GROUP,
        MA_CREATE_GROUP_ACK,
        MA_JOIN_GROUP,
        MA_JOIN_GROUP_ACK,
        MA_INVITE_GROUP,
        MA_INVITE_GROUP_ACK,
        MA_QUERY_GROUP_LIST,
        MA_GROUP_LIST,
        MA_QUERY_GROUP_INFO,
        MA_GROUP_INFO,
        MA_QUERY_GROUP_USERS,
        MA_GROUP_USERS,

*/



typedef bool (* LPFNPARSE)(int index, void * pack, int len);

typedef struct
{  
        unsigned int    uid;
        USERINFO        info;
        unsigned int    ulist[MAX_FRIENDS_COUNT];
        unsigned int    glist[MAX_GROUPS_COUNT];
}FILEUSERITEM;

typedef struct
{
        unsigned int    gid;
        GROUPINFO       info;
        unsigned int    list[MAX_GROUPMEMBERS_COUNT];
}FILEGROUPITEM;


static LPFNPARSE        lpfnParse[MA_MAXCOUNT];
static FILE *           fpu = NULL;
static FILE *           fpg = NULL;


#define PTCPARSEFUN(fun)        static bool fun (int index, void * pack, int len)

PTCPARSEFUN(_on_zero)
{
        // do nothing.
        return true;
}

PTCPARSEFUN(_on_login)
{
        MCPACK *        mcp;
        FILEUSERITEM    fui;
        unsigned int    uid;
        MCPACK          sendpack;

        
        mcp = (MCPACK *)pack;
        uid = atoi(mcp->regist.username);

        fseek(fpu, 0, SEEK_SET);
        
        while( fread(&fui, sizeof(FILEUSERITEM), 1, fpu) == 1 )
        {
                if( fui.uid == uid || strncmp(fui.info.username, mcp->regist.username, 32) == 0 )
                {
                        if( strncmp(fui.info.password, mcp->regist.password, 32) == 0 )
                        {
                                sendpack.action = MA_LOGIN_ANSWER;
                                sendpack.answer.state = 0;
                                sendpack.answer.uid = fui.uid;
                                sprintf(sendpack.answer.message, "Welcome back, %s.", fui.info.username);
                                ser_Send(index, &sendpack, sizeof(sendpack));
                                sock[index].state = 1;
                                memcpy(&(sock[index].userinfo), &fui.info, sizeof(USERINFO));

                        }else{
                                sendpack.action = MA_LOGIN_ANSWER;
                                sendpack.answer.state = 1;
                                sendpack.answer.uid = 0;
                                sprintf(sendpack.answer.message, "wrong password.");
                                ser_Send(index, &sendpack.answer.message, sizeof(sendpack));
                                sock[index].state = 0;
                        }
                        return true;
                }
        }
        
        sendpack.action = MA_LOGIN_ANSWER;
        sendpack.answer.state = 1;
        sendpack.answer.uid = 0;
        sprintf(sendpack.answer.message, "invalid username.");
        ser_Send(index, &sendpack, sizeof(sendpack));
        sock[index].state = 0;

        return false;

}

PTCPARSEFUN(_on_register)
{
        MCPACK      *   mcp;
        FILEUSERITEM    fui;
        MCPACK          sendpack;
        char            c;
        unsigned int    uid;


        mcp = (MCPACK *)pack;
        
        c = mcp->regist.username[0];
        if(c >= '0' && c <= '9')
        {
                sendpack.action = MA_REGISTER_ANSWER;
                sendpack.answer.uid = 0;
                sendpack.answer.state = 1;
                strcpy(sendpack.answer.message, "invalid username");
                ser_Send(index, &sendpack, sizeof(sendpack));
                return false;
        }
        

        fui.uid = 10000;

        fseek(fpu, 0, SEEK_SET);
        while( fread(&fui, sizeof(FILEUSERITEM), 1, fpu) == 1 )
        {
                if( strncmp(fui.info.username, mcp->regist.username, 32) == 0 )
                {
                        sendpack.action = MA_REGISTER_ANSWER;
                        sendpack.answer.uid = 0;
                        sendpack.answer.state = 1;
                        strcpy(sendpack.answer.message, "username exists.");
                        ser_Send(index, &sendpack, sizeof(sendpack));
                        return false;
                }
        }

        fseek(fpu, 0, SEEK_END);
        uid = fui.uid;
        uid ++;
        memset(&fui, 0, sizeof(fui));
        fui.uid = uid;
        fui.info.uid = uid;
        fui.info.age = mcp->regist.age;
        fui.info.gender = mcp->regist.gender;
        fui.info.head_pic = mcp->regist.head_pic;
        strncpy(fui.info.nickname, mcp->regist.nickname, 32);
        strcpy(fui.info.username, mcp->regist.username);
        strcpy(fui.info.password, mcp->regist.password);
        if( fwrite(&fui, sizeof(fui), 1, fpu) != 1 )
        {
                perror("fwrite");
                exit(1);
        }
        fflush(fpu);
        sendpack.action = MA_REGISTER_ANSWER;
        sendpack.answer.uid = uid;
        sendpack.answer.state = 0;
        strcpy(sendpack.answer.message, "success.");
        ser_Send(index, &sendpack, sizeof(sendpack));
        
        return false;
}

PTCPARSEFUN(_on_add_friend)
{
        MCPACK      *   mcp;
        unsigned int    uid;
        MCPACK          sendpack;
        int             i;
        bool            flag;


        mcp = (MCPACK *)pack;

        // 未登陆T出。
        if(sock[index].state == 0)
        {
                return false;
        }

        uid = sock[index].userinfo.uid;
        sendpack.action = MA_ADD_FRIEND;
        sendpack.request.uid = uid;

        // 消息发送给用户。
        flag = false;
        for(i=1; i<FD_SETSIZE; i++)
        {
                if(sock[i].sock == INVALID_SOCKET)
                {
                        continue;
                }
                
                if(sock[i].state == 0)
                {
                        continue;
                }

                if(sock[i].userinfo.uid == mcp->request.uid)
                {
                        flag = true;
                        ser_Send(i, &sendpack, sizeof(sendpack));
                }
        }

        // 不在线不给转发。
        if(!flag)
        {
                sendpack.action = MA_ADD_FRIEND_ANSWER;
                sendpack.answer.uid = 0;
                sendpack.answer.state = 1;
                strcpy(sendpack.answer.message, "user not online.");
                ser_Send(index, &sendpack, sizeof(sendpack));
        }

        return true;

}

PTCPARSEFUN(_on_add_friend_ack)
{
        MCPACK      *   mcp;
        MCPACK          sendpack;
        unsigned int    uid;
        int             i;
        bool            flag;
        FILEUSERITEM    fui;
        long            pos;


        mcp = (MCPACK *)pack;

        if(sock[index].state == 0)
        {
                return false;
        }
        
        // 玩家同意/拒绝添加好友请求。

        uid = sock[index].userinfo.uid;
        sendpack.action = MA_ADD_FRIEND_ANSWER;
        sendpack.answer.uid = uid;
        sendpack.answer.state = mcp->answer.state;
        strncpy(sendpack.answer.message, mcp->answer.message, 128);

        // uid同意mcp->uid的添加。
        flag = false;
        if(mcp->answer.state == 0)
        {
                fseek(fpu, 0, SEEK_SET);
                pos = ftell(fpu);
                while( fread(&fui, sizeof(FILEUSERITEM), 1, fpu) == 1 )
                {
                        if(fui.uid != mcp->answer.uid)
                        {
                                pos = ftell(fpu);
                                continue;
                        }

                        for(i=0; i<MAX_FRIENDS_COUNT; i++)
                        {
                                if(fui.ulist[i] == uid)
                                {
                                        sendpack.answer.state = 1;
                                        sendpack.answer.uid = uid;
                                        strcpy(sendpack.answer.message, "you have had this friend.");
                                        ser_Send(index, &sendpack, sizeof(sendpack));
                                        return true;
                                }
                        }

                        flag =true;
                        for(i=0; i<MAX_FRIENDS_COUNT; i++)
                        {
                                if(fui.ulist[i] == 0)
                                {
                                        fui.ulist[i] = uid;
                                        break;
                                }
                        }

                        // 好友列表已满。
                        if(i == MAX_FRIENDS_COUNT)
                        {
                                sendpack.answer.state = 1;
                                strcpy(sendpack.answer.message, "you reach the max friends count.");
                        }
                        
                        fseek(fpu, pos, SEEK_SET);
                        if( fwrite(&fui, sizeof(fui), 1, fpu) != 1 )
                        {
                                perror("fwrite");
                                exit(1);
                        }
                        fflush(fpu);
                        break;
                }
        }

        // 没有添加者。。错误的回应？
        if (!flag)
        {
                sock[index].error_pack_count ++;
                return true;
        }


        // 结果发回添国请求者。
        flag = false;
        for(i=1; i<FD_SETSIZE; i++)
        {
                if(sock[i].sock == INVALID_SOCKET || sock[i].state == 0)
                {
                        continue;
                }
                if(sock[i].userinfo.uid == mcp->answer.uid)
                {
                        flag = true;
                        ser_Send(i, &sendpack, sizeof(sendpack));
                }
        }

        // 玩家发送添加请求后未等响应直接离线。
        if( !flag )
        {
                // 不给保存回应信息。
        }
        
        return true;

}

PTCPARSEFUN(_on_friend_list)
{
        MCPACK          sendpack;
        unsigned int    uid;
        FILEUSERITEM    fui;
        bool            flag;
        int             i;
        int             j;

        
        if(sock[index].state == 0)
        {
                return false;
        }


        uid = sock[index].userinfo.uid;
        
        fseek(fpu, 0, SEEK_SET);

        flag = false;
        while( fread(&fui, sizeof(FILEUSERITEM), 1, fpu) == 1 )
        {
                if(fui.uid == uid)
                {
                        flag = true;
                        memset(&sendpack, 0, sizeof(sendpack));
                        sendpack.action = MA_FRIEND_LIST;
                        for(i=0, j=0; i<MAX_FRIENDS_COUNT; i++)
                        {
                                if(fui.ulist[i] != 0)
                                {
                                        sendpack.list.uid[j] = fui.ulist[i];
                                        j++;
                                }
                        }
                        ser_Send(index, &sendpack, sizeof(sendpack));
                }
        }
        
        if(!flag)
        {
                sock[index].error_pack_count ++;
        }

        return true;

}

PTCPARSEFUN(_on_user_info)
{
        MCPACK      *   mcp;
        MCPACK          sendpack;
        bool            flag;
        unsigned int    uid;
        FILEUSERITEM    fui;
        int             i;

        
        if(sock[index].state == 0)
        {
                return false;
        }

        mcp = (MCPACK *)pack;
        uid = sock[index].userinfo.uid;

        fseek(fpu, 0, SEEK_SET);
        flag = false;
        while( fread(&fui, sizeof(FILEUSERITEM), 1, fpu) == 1 )
        {
                if(fui.uid == mcp->request.uid)
                {
                        flag = true;
                        memset(&sendpack, 0, sizeof(sendpack));
                        sendpack.action = MA_ACK_USER_INFO;
                        memcpy(&(sendpack.userinfo), &(fui.info), sizeof(USERINFO));
                        memset(sendpack.userinfo.username, 0, 32);
                        memset(sendpack.userinfo.password, 0, 32);
                        for(i=1; i<FD_SETSIZE; i++)
                        {
                                if(sock[i].sock != INVALID_SOCKET)
                                {
                                        if(sock[i].userinfo.uid == mcp->request.uid)
                                        {
                                                sendpack.userinfo.onlinestate = sock[i].state;
                                        }
                                }
                        }
                        ser_Send(index, &sendpack, sizeof(sendpack));
                        break;
                }
        }

        if( ! flag )
        {
                sock[index].error_pack_count ++;
        }

        return true;

}

PTCPARSEFUN(_on_chat)
{
        MCPACK      *   mcp;
        MCPACK          sendpack;
        int             i;
        bool            flag;


        if(sock[index].state == 0)
        {
                return false;
        }
        
        mcp = (MCPACK *)pack;

        flag = false;
        for(i=1; i<FD_SETSIZE; i++)
        {
                if(sock[i].sock == INVALID_SOCKET || sock[i].state == 0)
                {
                        continue;
                }
        
                if(sock[i].userinfo.uid == mcp->chat.uid)
                {
                        sendpack.action = MA_CHAT;
                        memcpy(&(sendpack.chat), &(mcp->chat), sizeof(CHAT));
                        sendpack.chat.uid = sock[index].userinfo.uid;
                        ser_Send(i, &sendpack, sizeof(sendpack));
                        flag = true;
                }
        }

        // 对方不在线。。。
        if(!flag)
        {
        }

        return true;
}

PTCPARSEFUN(_on_online_user)
{
        MCPACK      *   mcp;
        MCPACK          sendpack;
        int             i;
        int             j;


        if(sock[index].state == 0)
        {
                return false;
        }
        
        mcp = (MCPACK *)pack;
        

        for(i=0, j=0; i<FD_SETSIZE; i++)
        {
                if(sock[i].sock == INVALID_SOCKET || sock[i].state == 0)
                {
                        continue;
                }
                memset(&sendpack, 0, sizeof(sendpack));
                sendpack.action = MA_ONLINE_USER;
                memcpy(&sendpack.userinfo, &sock[i].userinfo, sizeof(USERINFO));
                ser_Send(index, &sendpack, sizeof(sendpack));
                j++;
                if(j > 100)
                {
                        break;
                }
        }
        
        return true;

}

PTCPARSEFUN(_on_create_group)
{
        MCPACK      * mcp;
        mcp = (MCPACK *)pack;

        return false;
}

PTCPARSEFUN(_on_join_group)
{
        MCPACK      * mcp;
        mcp = (MCPACK *)pack;

        return false;
}

PTCPARSEFUN(_on_invite_group)
{
        MCPACK      * mcp;
        mcp = (MCPACK *)pack;

        return false;
}

PTCPARSEFUN(_on_invite_group_ack)
{
        MCPACK      * mcp;
        mcp = (MCPACK *)pack;
        return false;

}

PTCPARSEFUN(_on_group_list)
{
        MCPACK      * mcp;
        mcp = (MCPACK *)pack;
        return false;

}

PTCPARSEFUN(_on_group_info)
{
        MCPACK      * mcp;
        mcp = (MCPACK *)pack;
        return false;

}

PTCPARSEFUN(_on_group_users)
{
        MCPACK      * mcp;
        mcp = (MCPACK *)pack;
        return false;

}

#undef PTCPARSEFUN




static void ptc_init()
{
        int     i;
        for(i=0; i<MA_MAXCOUNT; i++)
        {
                lpfnParse[i] = _on_zero;
        }

        lpfnParse[MA_LOGIN_REQUEST] = _on_login;
        lpfnParse[MA_REGISTER_REQUEST] = _on_register;
        lpfnParse[MA_ADD_FRIEND] = _on_add_friend;
        lpfnParse[MA_ADD_FRIEND_ANSWER] = _on_add_friend_ack;
        lpfnParse[MA_QUERY_FRIEND_LIST] = _on_friend_list;
        lpfnParse[MA_QUERY_USER_INFO] = _on_user_info;
        lpfnParse[MA_CREATE_GROUP] = _on_create_group;
        lpfnParse[MA_JOIN_GROUP] = _on_join_group;
        lpfnParse[MA_INVITE_GROUP] = _on_invite_group;
        lpfnParse[MA_INVITE_GROUP_ACK] = _on_invite_group_ack;
        lpfnParse[MA_QUERY_GROUP_LIST] = _on_group_list;
        lpfnParse[MA_QUERY_GROUP_INFO] = _on_group_info;
        lpfnParse[MA_QUERY_GROUP_USERS] = _on_group_users;
        lpfnParse[MA_CHAT] = _on_chat;
        lpfnParse[MA_ONLINE_USER] = _on_online_user;


        fpu = fopen("users.bin", "rb+");
        if(fpu == NULL)
        {
                perror("fopen users.bin ");
                printf("try to create one\n");
                fpu = fopen("users.bin", "wb+");
                if(fpu == NULL)
                {
                        perror("fopen users.bin ");
                        exit(1);
                }
        }

        fpg = fopen("groups.bin", "rb+");
        if(fpg == NULL)
        {
                perror("fopen groups.bin ");
                printf("try to create one\n");
                fpg = fopen("groups.bin", "wb+");
                if(fpg == NULL)
                {
                        perror("fopen groups.bin ");
                        exit(1);
                }
        }


}

/////////////////////////////////////////////////////////////////////////////

bool ptc_parse(int index, void * pack, int len)
{
        MCPACK      *   mcp;
        static bool     inited = false;
        bool            ret;

        if(!inited)
        {
                ptc_init();
                inited = true;
        }

        mcp = (MCPACK *)pack;
        
        if(mcp->action >= MA_MAXCOUNT)
        {
                sock[index].error_pack_count ++;
                printf("error ptc: %d from %s\n>", mcp->action, inet_ntoa(sock[index].addr.sin_addr));
		ret = true;
        }else{
                ret = lpfnParse[mcp->action](index, pack, len);
        }

        if(sock[index].error_pack_count >= MAX_ERROR_PACK_COUNT)
        {
                return false;  
        }
        return ret;

}
