


#include "gprs.h"

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/socket.h>
#include <termios.h>
#include <errno.h>
#include <string.h>
#include <assert.h>
#include <sys/select.h>
#include <time.h>
#include <vector>
#include <string>
#include <map>
#include <algorithm>
#include <iostream>
#include <sstream>
#include <fstream>

using namespace std; 

//#define usleep(x)       (void)(0)
//#define sleep(x)        (void)(0)


CGPRS::CGPRS(const char * devfile)
{


        struct termios  opts; 

        cerr << "open " << devfile << endl;
        fd = open(devfile,  O_RDWR);// | O_NOCTTY | O_NDELAY); 
        if(fd < 0)
        {
                perror("open"); 
                fflush(stderr);
                throw __LINE__; 
        }

        cerr << "opened." << endl;

        tcgetattr(fd,  &opts); 
        cfmakeraw(&opts);
        cfsetispeed(&opts,  B115200); 
        cfsetospeed(&opts,  B115200); 
        tcflush(fd,  TCIFLUSH); 
        tcsetattr(fd,  TCSANOW,  &opts); 

        /*
        opts.c_cflag &= ~CSIZE; 
        opts.c_cflag |= CS8; 
        opts.c_iflag |= IGNPAR; 
        opts.c_cflag &= ~CSTOPB; 
        opts.c_cflag &= ~CRTSCTS; 

        tcflush(fd, TCIOFLUSH);
        */

        waitingreply = false;

        string      line; 

        sock = 0; 

        line = "AT\r";
        sendqueue.push_back(line);
        line = "AT+CSQ\r";
        sendqueue.push_back(line);
        line = "AT+CSCS=\"GSM\"\r"; 
        sendqueue.push_back(line); 
        line = "AT+CMGF=0\r"; 
        sendqueue.push_back(line); 


//        SendSMS("13591991768", "hello");
//        SendSMS("13591991768", "中语言");

        /*

        for(int i=1; i<10; i++)
        {
                stringstream    sst;
                sst.str("");
                sst << i;
                lastsmsid = sst.str();
                smsidqueue.push_back(lastsmsid);
                line = "AT+CMGR=" + sst.str() + "\r";
                sendqueue.push_back(line);
                line = "AT+CMGD=" + sst.str() + "\r";
                sendqueue.push_back(line);
        }
        */

        selectproc(); 

}


CGPRS::~CGPRS()
{
        close(fd); 

}


void CGPRS::SendSMS(const char * phno,  const char * message) 
{
        std::string     line;
        std::string     smsc = "8613800100500";
        std::string     dstphno;
        std::string     sms;
        char  *         buffer;


        smsc = ReserveOEWithF(smsc);
        dstphno = ReserveOEWithF(phno);
        sms = utf82unistr(message);

        buffer = (char *)malloc(dstphno.length() + sms.length() + 100);

        sprintf(buffer, "AT+CMGS=%d\n", sms.length() / 2 + 15);
        line = buffer;
        //cerr << "push " << line << endl;
        //
        
        sendqueue.push_back(line);
        
        sprintf(buffer, "001100%02x9168%s000801%02x%s\x1a",
                strlen(phno) + 2,
                dstphno.c_str(),
                sms.length() / 2,
                sms.c_str());
        line = buffer;
        sendqueue.push_back(line);


       //cerr << "push " << line << endl;


        /*
        sendbuff = *sendqueue.begin();
        sendqueue.erase(sendqueue.begin());

        Write(sendbuff);
        */

}



void CGPRS::selectproc()
{
        int         ret; 
        int         maxfd; 
        struct timeval  timeout; 

        maxfd = (fd>sock)?(fd):(sock); 
        maxfd++; 

        cerr << "maxfd=" << maxfd << endl;
        fflush(stderr);

        /*
        sendbuff = * sendqueue.begin();
        sendqueue.erase(sendqueue.begin());

        Write(sendbuff);
        */

        while(true)
        {
                FD_ZERO(&fdsr); 
                FD_ZERO(&fdsw); 
                FD_SET(fd, &fdsw);
                FD_SET(fd, &fdsr);
                FD_SET(STDIN_FILENO, &fdsr);
 
                timeout.tv_sec = 3; 
                timeout.tv_usec = 0; 

                ret = select(maxfd, &fdsr,  &fdsw,  NULL, &timeout);
                if(ret < 0)
                {
                        perror("select"); 
                        fflush(stderr);
                        exit(0); 
                }
                else if(ret  ==  0)
                {
                        cerr << "timeout" << endl;
                        fflush(stderr);
                        continue;
                }
                else
                {
                        ret = _stdinpoll();
                        ret = _sendpoll();
                        ret = _recvpoll(); 
                }
        }

}

int CGPRS::_recvpoll()
{
        if(!FD_ISSET(fd,  &fdsr))
        {
                return 0; 
        }

        char    buffer[1024]; 
        ssize_t bytesread; 
        int     ret;

        memset(buffer,  0,  1024); 
        bytesread = 0;
        //while(true)
        {
                ret = read(fd,  buffer+bytesread,  1024-bytesread); 
                if(ret < 0)
                {
                        perror("read");
                }
                bytesread+=ret;
        }

        assert(bytesread >= 0); 
        if(bytesread  ==  0)
        {
                usleep(100000);
                return 0; 
        }

        //cerr << bytesread << "bytes read. " << buffer << endl;
        cerr << buffer;
        fflush(stderr);

        readpool += buffer;

        std::string     lastmsg;
        stringstream    sst;
        string          line;
        lastmsg = "";

        if(!sendqueue.empty())
        {
                lastmsg = *sendqueue.begin();
        }

        if((int)echomessage.find_last_of("\r\n") >= 0)
        {
                echomessage.erase(echomessage.find_last_of("\r\n"));
        }

        while(!readpool.empty())
        {
                readpool.erase(0, readpool.find_first_not_of("\r\n"));

                if(!echomessage.empty())
                {
                        if(readpool.length() >= echomessage.length())
                        {
                                if(readpool.substr(0, echomessage.length()) == echomessage)
                                {
                                        readpool.erase(0, echomessage.length());
                                        continue;
                                }
                        }
                        else
                        {
                                if(readpool.empty())
                                {
                                        return 0;
                                }
                                if(readpool == echomessage.substr(0, readpool.length()))
                                {
                                        echomessage.erase(0, readpool.length());
                                        readpool.clear();
                                        return 0;
                                }
                        }
                }

                if(readpool.length() >= 2 && readpool.substr(0, 2) == "OK")
                {
                        readpool.erase(0, 2);
                        waitingreply = false;
                }
                else if(readpool.length() >= 5 && readpool.substr(0, 5) == "ERROR")
                {
                        cerr << "ERROR!" << endl;
                        fflush(stderr);
                        waitingreply = false;

                        readpool.erase(0, 5);
                        continue;
                }
                else if(readpool.length() >= 4 && readpool.substr(0, 4) == "RING")
                {
                        line = "ATH\r";
                        sendqueue.insert(sendqueue.begin(), line);
                        //cerr << "RINGING... Hunged up.\n";
                        readpool.erase(0, 4);
                }
                else if(readpool.length() >= 2 && readpool.substr(0, 2) == "> ")
                {
                        //tcflush(fd, TCIOFLUSH);
                        readpool.erase(0, 2);
                }
                else if(readpool.length() >= 6 && readpool.substr(0, 6) == "+CMGS:")
                {
                        if((int)readpool.find_first_of("\r\n") == -1)
                        {
                                return 0;
                        }
                        readpool.erase(0, readpool.find_first_of("\r\n"));
                        continue;
                }
                else if(readpool.length() >= 5 && readpool.substr(0, 5) == "+CSQ:")
                {
                        if((int)readpool.find_first_of("\r\n") == -1)
                        {
                                return 0;
                        }
                        readpool.erase(0, readpool.find_first_of("\r\n"));
                        continue;
                }
                else if(readpool.length() >= 6 && readpool.substr(0, 6) == "+CMTI:")
                {
                        if((int)readpool.find_first_of("\r\n") == -1 )
                        {
                                return 0;
                        }
                        lastsmsid = readpool.substr(readpool.find_first_of(",") + 1);
                        lastsmsid.erase(lastsmsid.find_first_of("\r\n"));

                        line = "AT+CMGR=" + lastsmsid + "\r";
                        /*
                        if(sendqueue.empty())
                        {
                                sendqueue.push_back("");
                        }
                        */
                        sendqueue.push_back(line);
                        smsidqueue.push_back(lastsmsid);

                        readpool.erase(0, readpool.find_first_of("\r\n"));

                }
                else if(readpool.length() >= 6 && readpool.substr(0, 6) == "+CMGR:")
                {
                        if(strstr(readpool.c_str(), "OK") == NULL)
                        {
                                return 0;
                        }

                        string  smsct;

                        if(strstr(readpool.c_str(), "REC") != NULL)
                        {
                                string  ph;
                                string  ct;

                                lastsmsid = *smsidqueue.begin();
                                smsidqueue.erase(smsidqueue.begin());
                                cerr << "收到的短信id:" << lastsmsid << endl;
                                cerr << "内容串:" << readpool << endl;
                                fflush(stderr);
                        
                                ph = readpool.substr(readpool.find_first_of(",") + 2);
                                ph.erase(0, ph.find_first_not_of("+"));
                                ph.erase(ph.find_first_of("\""));
                                cerr << "Sender:" << ph << endl;
                                ct = readpool.substr(readpool.find_first_of("\r\n"));
                                ct.erase(0, ct.find_first_not_of("\r\n"));
                                ct.erase(ct.find_first_of("\r\n"));
                                ct = unistr2utf8(ct);
                                cerr << "Message:" << ct << endl;
                                cout << "SMS," << ph << "," << ct << endl;
                                fflush(stdout);
                                readpool.erase(0, strstr(readpool.c_str(), "OK") - readpool.c_str());
                                readpool.erase(0, 2);

                                string line;
                                line = "AT+CMGD=" + lastsmsid + "\r";
                                sendqueue.push_back(line);

                        }
                        else
                        {

                                smsct = readpool.substr(readpool.find_first_of("\r\n"));
                                smsct.erase(0, smsct.find_first_not_of("\r\n"));
                                smsct.erase(smsct.find_first_not_of("0123456789ABCDEF"));

                                assert(!smsidqueue.empty());

                                lastsmsid = *smsidqueue.begin();
                                smsidqueue.erase(smsidqueue.begin());
                                cerr << "收到的短信id:" << lastsmsid << endl;
                                cerr << "内容串:" << smsct << endl;
                                fflush(stderr);
                                
                                readpool.erase(0, strstr(readpool.c_str(), "OK") - readpool.c_str());
                                readpool.erase(0, 2);


                                // 处理收到的短信串.
                                char    temp[100];
                                int     smsclen;        // 短信中心号码长度 
                                string  smsfmt;         // 短信中心号码格式
                                string  smsc;           // 短信中心号码
                                string  tpmti_mms_rp;
                                int     senderlen;
                                string  senderfmt;      // 91=+, A1=飞信
                                string  sender;
                                string  tppid;          // 00=GSM
                                string  tpdcs;          // 08=ucs2
                                string  timestap; 
                                int     msglen;
                                string  message;

                                strncpy(temp, smsct.c_str(), 2);
                                temp[2] = '\0';
                                sscanf(temp, "%x", &smsclen);
                                smsct.erase(0, 2);

                                smsfmt = smsct.substr(0, 2);
                                smsct.erase(0, 2);

                                smsc = smsct.substr(0, smsclen*2-2);
                                smsc = ReserveOEWithF(smsc);
                                smsc.erase(smsc.find_last_not_of("F") + 1);
                                smsct.erase(0, smsclen*2-2);

                                cerr << "短信中心:" << smsc << endl;

                                tpmti_mms_rp = smsct.substr(0, 2);
                                smsct.erase(0, 2);

                                cerr << "基本参数:" << tpmti_mms_rp << endl;

                                strncpy(temp, smsct.c_str(), 2);
                                temp[2] = '\0';
                                sscanf(temp, "%x", &senderlen);
                                smsct.erase(0, 2);

                                senderfmt = smsct.substr(0, 2);
                                smsct.erase(0, 2);
                                
                                int     t = senderlen;
                                if(2 * (t/2) != t)      t++;
                                sender = smsct.substr(0, t);
                                sender = ReserveOEWithF(sender);
                                sender.erase(sender.find_last_not_of("F") + 1);
                                smsct.erase(0, t);

                                cerr << "发送者:" << sender << endl;
                                
                                tppid = smsct.substr(0, 2);
                                smsct.erase(0, 2);

                                cerr << "协议标识:" << tppid << endl;

                                tpdcs = smsct.substr(0, 2);
                                smsct.erase(0, 2);

                                cerr << "编码方式:" << tpdcs << endl;

                                timestap = smsct.substr(0, 14);
                                smsct.erase(0, 14);

                                strncpy(temp, smsct.c_str(), 2);
                                temp[2] = '\0';
                                sscanf(temp, "%x", &msglen);
                                smsct.erase(0, 2);

                                message = smsct;
                                message = unistr2utf8(message);

                                cerr << "短信内容:" << message << endl << endl;


                                cout << "SMS," << sender << "," << message << endl;
                                fflush(stdout);

                                string line;
                                line = "AT+CMGD=" + lastsmsid + "\r";
                                /*
                                if(sendqueue.empty())
                                {
                                        sendqueue.push_back("");
                                }
                                */
                                sendqueue.push_back(line);
                        }

                }
                else
                {
                        if(readpool.empty())
                        {
                                return 0;
                        }

                        if((int)readpool.find_first_of("\r\n") == -1)
                        {
                                return 0;
                        }


                        cerr << "Unidentified Command " << readpool.substr(0, readpool.find_first_of("\r\n")) << endl;
                        fprintf(stderr, "%x\n", readpool.at(0));
                        fflush(stderr);
                        
                        //readpool.erase(0, readpool.find_first_of("\r\n"));
                        readpool.erase(0, 1);
                        continue;
                }

                /*
                if(!sendqueue.empty())
                {
                        sendqueue.erase(sendqueue.begin());
                }
                */
                if(!sendqueue.empty())
                {
                        sendbuff = *sendqueue.begin();
                        sendqueue.erase(sendqueue.begin());
                        if(sendbuff == "\x1a")
                        {
                                waitingreply = false;
                        }
                        else
                        {
                                waitingreply = true;
                        }

                       // Write(sendbuff);
                }
                break;
        }

        return 1; 

}

void CGPRS::Write(std::string  buf)
{
        int     ret;
        cerr << buf;
        ret = write(fd, buf.c_str(), buf.length());
        if(ret < 0)
        {
                perror("write");
        }
}


int CGPRS::_sendpoll()
{

        static std::string      lastmsg;
        static time_t           tmval = (time_t)0;

        if(!FD_ISSET(fd,  &fdsw))
        {
                return 0; 
        }
    
        if(sendbuff.empty())
        {
                if(waitingreply)
                {
                        usleep(1000);
                        if(tmval == (time_t)0)
                        {
                                return 0;
                        }
                        if(time(NULL) - tmval > 100)
                        {
                                if(sendqueue.empty())
                                {
                                        sendqueue.push_back(lastmsg);
                                }else{
                                        sendqueue.insert(sendqueue.begin(), lastmsg);
                                }
                        }
                        else
                        {
                                return 0;
                        }
                }
                if(sendqueue.empty())
                {
                        usleep(100000);
                        return 0;
                }
                sendbuff = * sendqueue.begin();
                sendqueue.erase(sendqueue.begin());
                waitingreply = true;
        }

        usleep(1000);

        //cerr << "prepare to write: " << sendbuff << endl;
        cerr << sendbuff << endl;
        fflush(stderr);

        unsigned int    bytes_written;
        int             ret;

        bytes_written = 0;
        while(bytes_written < sendbuff.length())
        {
                ret = write(fd, sendbuff.c_str()+bytes_written, sendbuff.length()-bytes_written);
                if(ret < 0)
                {
                        perror("write");
                        return -1;
                }
                bytes_written += (unsigned int)ret;
        }

        tmval = time(NULL);
        lastmsg = sendbuff;
        echomessage += lastmsg;

        /*
        if((bytes_written = write(fd,  sendbuff.c_str(),  sendbuff.length())) < 0)
        {
                perror("write");
                return -1;
        }
        */
        //cerr << bytes_written << " written." << endl;
 
        usleep(10000);
        sendbuff.clear();
        return 0;

}

int CGPRS::_stdinpoll()
{
        if(!FD_ISSET(STDIN_FILENO, &fdsr))
        {
                return 0;
        }

        
        std::string     temp;
        std::string     phno;
        std::string     cont;

        char            buffer[1024];

        //cin.getline(buffer, 1024);
        fgets(buffer, 1024, stdin);
        temp = buffer;

        temp.erase(0, temp.find_first_not_of(" \t"));
        
        if(temp.empty())
        {
                return 0;
        }

        if(temp.length() >= 4 && temp.substr(0, 4) == "exit")
        {
                exit(0);
        }

        if(temp.length() > 4 && temp.substr(0, 3) == "SMS")
        {
                if(temp.at(3) != ',')
                {
                        cerr << "Error Input" << endl;
                        fflush(stderr);
                        return -1;
                }
                temp = temp.erase(0, 4);
                if((int)temp.find_first_of(",") == (-1))
                {
                        cerr << "Error Input" << endl;
                        fflush(stderr);
                        return -1;
                }

                phno = temp.substr(0, temp.find_first_of(","));
                cont = temp.substr(temp.find_first_of(",") + 1);
                if((int)phno.find_first_not_of("0123456789") != (-1))
                {
                        cerr << "Error Input" << endl;
                        fflush(stderr);
                        return -1;
                }
                if(cont.empty())
                {
                        cerr << "Error Input" << endl;
                        fflush(stderr);
                        return -1;
                }


                SendSMS(phno.c_str(), cont.c_str());
                
                return 0;
        }

        std::string     line = buffer;

        //Write(line);

        sendqueue.push_back(line);

        return -1;

}

 
std::string CGPRS::utf82unistr(std::string utfstr)
{
        const char *    p = utfstr.c_str();
        char *          result = NULL;
        char *          tmp = NULL;
        int             temp;

        char            t1;
        char            t2;
        char            t3;

        result = (char *)malloc(utfstr.length() * 4 + 2);
        memset(result, 0, utfstr.length() * 4 + 2);
        tmp = result;

        while(*p != '\0')
        {
                if(*p > 0)
                {
                        temp = *p;
                        temp &= 0xff;
                        sprintf(tmp, "00%02x", temp);
                        tmp += 4;
                }
                else if( ((*p & (0xff << 5)) & 0xff)== 0xc0 )
                {
                        t1 = *p & (0xff >> 3);
                        p++;
                        t2 = *p & (0xff >> 2);

                        temp = t1 >> 2;
                        temp &= 0xff;
                        sprintf(tmp, "%02x", temp);
                        tmp += 2;

                        temp = t2 | ((t1 & (0xff >> 6)) << 6);
                        temp &= 0xff;
                        sprintf(tmp, "%02x", temp);
                        tmp += 2;


                }
                else if( ((*p & (0xff << 4) ) & 0xff) == 0xe0 )
                {
                        t1 = *p & (0xff >> 4);
                        p++;
                        t2 = *p & (0xff >> 2);
                        p++;
                        t3 = *p & (0xff >> 2);

                        temp = (t1 << 4) | (t2 >> 2);
                        temp &= 0xff;
                        sprintf(tmp, "%02x", temp);
                        tmp += 2;
                        
                        temp = ((t2 & (0xff>>6)) << 6) | t3;
                        temp &= 0xff;
                        sprintf(tmp, "%02x", temp);
                        tmp += 2;

                }
                else
                {
                        assert(0);
                }
                p++;
        }

        *tmp = '\0';

        std::string     res = result;

        free(result);

        return res;


}

std::string CGPRS::unistr2utf8(std::string unistr)
{

        int             len;
        const char *    p;
        char    *       result;
        char    *       tmp;
        int             t;
        int             uni;
        char            temp[10];


        len = unistr.length();
        assert(2 *(len/2) == len);

        p = unistr.c_str();
        result = (char *)malloc(len / 4 * 3 + 2);
        tmp = result;

        while(*p != '\0')
        {
                memset(temp, 0, sizeof(temp));
                strncpy(temp, p, 2);
                p += 2;
                temp[2] = '\0';
                sscanf(temp, "%x", &t);
                strncpy(temp, p, 2);
                p += 2;
                temp[2] = '\0';
                sscanf(temp, "%x", &uni);
                
                t &= 0xff;
                uni &= 0xff;
                uni = (t << 8) | uni;

                if(uni >= 0x0000 && uni <= 0x007f)
                {
                        *tmp = (char)uni;
                        tmp ++;
                }
                else if(uni >= 0x0080 && uni <= 0x07ff)
                {
                        *tmp = 0xc0 | (uni >> 6);
                        tmp ++;
                        *tmp = 0x80 | (uni & (0xff >> 2));
                        tmp ++;
                }
                else if(uni >= 0x0800 && uni <= 0xffff)
                {
                        *tmp = (0xe0 | (uni >> 12));
                        tmp ++;
                        *tmp = (0x80 | ((uni >> 6) & (0x00ff>>2)));
                        tmp ++;
                        *tmp = (0x80 | (uni & (0xff >> 2)));
                        tmp ++;
                }
        }

        *tmp = '\0';

        std::string     res = result;
        free(result);
        return res;
}

std::string CGPRS::ReserveOEWithF(std::string str)
{
        int             len;
        int             i;
        std::string     result;

        len = str.length();

        if(2 * (len/2) != len)
        {
                str += "F";
                len ++;
        }

        for(i=0; i<len; i+=2)
        {
                result += str.at(i+1); 
                result += str.at(i);
        }

        return result;

}















