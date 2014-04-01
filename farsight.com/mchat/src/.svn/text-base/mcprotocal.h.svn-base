

#pragma once
#ifndef __MCPROTOCAL_H__
#define __MCPROTOCAL_H__



#define MAX_FRIENDS_COUNT               100
#define MAX_GROUPS_COUNT                100
#define MAX_GROUPMEMBERS_COUNT          100



#pragma pack(push)
#pragma pack(1)

typedef enum MCACTION
{
        MA_ZERO		= 0,
        MA_ERROR,
        MA_LOGIN_REQUEST,
        MA_LOGIN_ANSWER,
        MA_REGISTER_REQUEST,
        MA_REGISTER_ANSWER,
        MA_ADD_FRIEND,                  // 用户1->服务器->用户2
        MA_ADD_FRIEND_ANSWER,           // 用户2->服务器->用户1
        MA_QUERY_FRIEND_LIST,           
        MA_FRIEND_LIST,
        MA_QUERY_USER_INFO,
        MA_ACK_USER_INFO,

        MA_CHAT,
        MA_ONLINE_USER,

        // 未实现。
        MA_MODIFY_UINFO,
        
        MA_CREATE_GROUP,
        MA_CREATE_GROUP_ACK,
        MA_JOIN_GROUP,
        MA_JOIN_GROUP_ACK,
        MA_INVITE_GROUP,                // 用户1->服务器->用户2
        MA_INVITE_GROUP_ACK,            // 用户2->服务器->用户1
        MA_QUERY_GROUP_LIST,
        MA_GROUP_LIST,
        MA_QUERY_GROUP_INFO,
        MA_GROUP_INFO,
        MA_QUERY_GROUP_USERS,
        MA_GROUP_USERS,

        // 未实现。
        MA_MODIFY_GINFO,
	
        // MA_MAXCOUNT必须是MCACTION的最后一个常量值。
        MA_MAXCOUNT
}MCACTION;

typedef struct USERINFO
{
        char            username[32];
        char            password[32];

        unsigned int    uid;
        char            nickname[32];
        int             age;
        int             gender;
        int             head_pic;
        int             onlinestate;
}USERINFO;

typedef struct GROUPINFO
{
        char            groupname[32];
        unsigned int    gid;
        unsigned int    ownerid;
        int             usercount;
        char            message[128];
}GROUPINFO;



typedef struct LOGIN_REGISTER
{
        char            username[32];
        char            password[32];
        char            nickname[32];
        int             age;
        int             gender;
        int             head_pic;
        
}LOGIN_REGISTER;

typedef struct COMMON_ANSWER
{
        int             state;  // 0-Success, 1-fail
        unsigned int    uid;    // 帐号
        char            message[128];
}COMMON_ANSWER;

typedef struct SINGLE_USER_REQUEST
{
        unsigned int    uid;
}SINGLE_USER_REQUEST;

typedef struct FRIEND_LIST
{
        unsigned int    uid[MAX_FRIENDS_COUNT]; 
}FRIEND_LIST;

typedef struct CHAT
{
        unsigned int    uid;
        unsigned int    gid;
        char            message[1024];
}CHAT;



typedef struct GROUP_CREATE_REQUEST
{
        char            groupname[32];
}GROUP_CREATE_REQUEST;

typedef struct GROUP_JOIN_REQUEST
{
        int             gid;
}GROUP_JOIN_REQUEST;

typedef struct GROUP_INVITE_REQUEST
{
        int             uid;
        int             gid;
        char            message[128];
}GROUP_INVITE_REQUEST;

typedef struct GROUP_LIST_ACK
{
        unsigned int    gid[MAX_GROUPS_COUNT];
}GROUP_LIST_ACK;



typedef struct GROUP_USERS
{
        unsigned int    uid[MAX_GROUPMEMBERS_COUNT];
}GROUP_USERS;


typedef COMMON_ANSWER           GROUP_JOIN_ACK;
typedef GROUP_JOIN_REQUEST      GROUP_INFO_REQUEST;
typedef GROUP_INVITE_REQUEST    GROUP_INVITE_ACK;
typedef GROUP_JOIN_REQUEST      GROUP_USERS_REQUEST;
typedef COMMON_ANSWER           GROUP_CREATE_ACK;

	
typedef struct MCPACK
{
        MCACTION	action;
        union
        {
                LOGIN_REGISTER          regist;
                COMMON_ANSWER           answer;
                SINGLE_USER_REQUEST     request;
                FRIEND_LIST             list;
                USERINFO                userinfo;
                CHAT                    chat;
                GROUP_CREATE_REQUEST    groupcreate;
                GROUP_JOIN_REQUEST      groupid;
                GROUP_INVITE_REQUEST    groupinvite;
                GROUP_LIST_ACK          grouplist;
                GROUPINFO               groupinfo;
                GROUP_USERS             groupusers;
        };
}MCPACK;


#pragma pack(pop)


#endif

