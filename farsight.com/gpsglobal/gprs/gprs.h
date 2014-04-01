

#pragma once

#ifndef __GPRS_H__
#define __GPRS_H__



#include <vector>
#include <string>
#include <sys/select.h>
#include <sys/types.h>

class CGPRS
{

public:
        CGPRS(const char * devfile); 
        ~CGPRS(); 


        void SendSMS(const char * phno,  const char * message); 

        void Write(std::string  buf);


private: 
        int     fd; 
        int     sock; 


        std::string     lastsmsid;

        std::string     echomessage;


        fd_set      fdsr; 
        fd_set      fdsw; 
 
        std::vector<std::string>    sendqueue; 
        std::vector<std::string>    smsidqueue;

        std::string     readpool; 
        std::string     sendbuff;

    
        void selectproc(); 


        int _recvpoll(); 
        int _sendpoll(); 
        int _stdinpoll();
        int _parsemsg(); 
        

        bool    waitingreply;


        std::string     utf82unistr(std::string utf8);
        std::string     unistr2utf8(std::string unistr);

        std::string     ReserveOEWithF(std::string str);

}; 


#endif


