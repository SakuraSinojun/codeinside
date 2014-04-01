

#include "log_tracer.h"
#include "iptv_logging.h"

#ifdef _WIN32
#include <windows.h>
#endif

#include <cstring>
#include <cerrno>
#include <algorithm>
using namespace std;


namespace iptv_logging {

static IptvLog * g_IptvLog = NULL;
IptvLog * IptvLog::GetInstance()
{
    if (!g_IptvLog)
        g_IptvLog = new IptvLog();
    return g_IptvLog;
}

IptvLog::IptvLog() {
}

IptvLog::~IptvLog() {
}

void IptvLog::AddTracer(LogTracer* rec) {
    rec_list_.push_back(rec);
}

void IptvLog::RemoveTracer(LogTracer* rec) {
    LogTracerIterator iter;
    for(iter = rec_list_.begin(); iter != rec_list_.end(); iter ++) {
        if(*iter == rec)
            break;
    }
    if(iter != rec_list_.end())
        rec_list_.erase(iter);
}

LogTracer* IptvLog::RemoveTracer(string recname) {
    LogTracerIterator iter;
    for(iter = rec_list_.begin(); iter != rec_list_.end(); iter ++) {
        if((*iter)->get_name_() == recname)
            break;
    }
    if(iter != rec_list_.end()) {
        rec_list_.erase(iter);
        return *iter;
    }
    return NULL;
}

void IptvLog::Output(const char * log, IptvLogMessage::RawMessage& msg)
{
    LogTracerIterator iter;

    for (iter = rec_list_.begin(); iter != rec_list_.end(); iter++) {
        (*iter)->SetLogged(true);
        (*iter)->Output(log, msg);
        (*iter)->SetLogged(false);
    }

}

string IptvLog::GetLastErrorMessage(void)
{
#if defined(WIN32) || defined(_WIN32)
    DWORD dw = ::GetLastError();
    LPVOID  lpMsgBuf;
    FormatMessageA(
        FORMAT_MESSAGE_ALLOCATE_BUFFER |
        FORMAT_MESSAGE_FROM_SYSTEM |
        FORMAT_MESSAGE_IGNORE_INSERTS,
        NULL,
        dw,
        MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), // Default language
        (LPSTR) &lpMsgBuf,
        0,
        NULL
        );
    string  str = string((char *)lpMsgBuf);
    LocalFree(lpMsgBuf);
    return str;
#else
    return strerror(errno);
#endif

}
}

