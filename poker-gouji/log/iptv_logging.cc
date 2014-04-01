

// #include "config.h"
#include "iptv_logging.h"
#include "log_tracer.h"
#include "log.h"

#include <cstdlib>
#include <cstdio>
#include <cstring>
#include <stdarg.h>
#include <ctime>
#include <string>
#include <sstream>
#include <iomanip>
#include <time.h>

using namespace std;

#ifdef _WIN32
#include <windows.h>
#else
#include <unistd.h>
#include <sys/sysinfo.h>
int GetTickCount()
{
    struct sysinfo  si;
    sysinfo(&si);
    return (int)si.uptime;
}
#endif

namespace iptv_logging {

IptvLogMessage::RawMessage::RawMessage(const char * file, 
    const char * func, int line, IptvLogType type, 
    IptvLogLevel level, const char * module)
    : file_(file)
    , func_(func)
    , line_(line)
    , type_(type)
    , level_(level)
    , module_(module)
{
}

IptvLogMessage::IptvLogMessage(const char * file, const char * func, int line,
    IptvLogType type, IptvLogType level, const char * module) :
    raw_message_(file, func, line, type, level, module)
{
}

IptvLogMessage::IptvLogMessage(const char * file, const char * func, int line, const char * module) :
    raw_message_(file, func, line, LOGTYPE_USER, LOGLEVEL_DEBUG, module)
{
}

IptvLogMessage::IptvLogMessage() :
    raw_message_("", "", 0, LOGTYPE_USER, LOGLEVEL_DEBUG, "")
{
}

IptvLogMessage::~IptvLogMessage() {
    // cut down the file path.
    size_t  pos = raw_message_.file_.find_last_of("/\\");
    if(pos != string::npos) {
        raw_message_.file_ = raw_message_.file_.substr(pos + 1);
    }

    ostringstream   oss;

    oss << '[';
    time_t      t = time(NULL);
    struct tm * local_time = localtime(&t);
    oss << std::setfill('0')
#if 0
        << std::setw(2) << 1 + local_time.tm_mon
        << '-'
        << std::setw(2) << local_time.tm_mday
        << ' '
#endif
        << std::setw(2) << local_time->tm_hour
        << ':'
        << std::setw(2) << local_time->tm_min
        << ':'
        << std::setw(2) << local_time->tm_sec
        << ':';
    oss << std::setw(3) << (GetTickCount() % 1000);
    oss << ']';

    if (!raw_message_.file_.empty()) {
        oss << '[';
        oss << raw_message_.file_;
        oss << ':' << raw_message_.func_;
        oss << ':' << raw_message_.line_;
        oss << ']';
    }

    if(raw_message_.module_.length() > 0) {
        oss << '<' << raw_message_.module_ << '>';
    }

    string  st = stream_.str();
    pos = st.find_last_not_of("\r\n ");
    if (pos != string::npos)
        st = st.substr(0, pos + 1);

    oss << ' ' << st;
    oss << std::endl;
    /*
    if (oss.str().at(oss.str().length() - 1) != '\r'
        && oss.str().at(oss.str().length() - 1) != '\n')
    {
        oss << std::endl;
    }
    */

    IptvLog::GetInstance()->Output(oss.str().c_str(), raw_message_);

    if (raw_message_.type_ == LOGTYPE_FATAL) {
        abort();
    }

    if (raw_message_.type_ == LOGTYPE_FATAL) {
        abort();
    }
}


static const char * g_module_list[100] = {NULL};
static int          g_module_count = 0;

// this class will be constructed before main() is called.
// so we'd better not use vector or list.
ModuleListHelper::ModuleListHelper(const char * name)
    : module_name_(name)
{
    if (g_module_count >= 100)
        return;
    for (int i = 0; i < g_module_count; i++) {
        if (strcmp(g_module_list[i], name) == 0)
            return;
    }
    g_module_list[g_module_count++] = name;
}

void ModuleListHelper::OutputAllModules(void)
{
    for (int i = 0; i < g_module_count; i++) {
        ::printf("\t%s\n", g_module_list[i]);
    }
}

} // namespace iptv_logging


// extern for "C"..
int log_trace(const char * file, const char * func, int line,
    IptvLogType type, IptvLogLevel level, const char * module,
    const char * fmt, ...)
{
    va_list arglist;
    char    temp[1024];

    va_start(arglist, fmt);
    int     ret = ::vsnprintf(temp, sizeof(temp), fmt, arglist);
    va_end(arglist);
    if (ret <= 0) {
        ::printf("vsnprintf failed.\n");
    }
    else if (ret < (int)sizeof(temp)) {
        iptv_logging::IptvLogMessage(file, func, line, type, level, module).stream() << temp;
    }
    else {
        char * buffer = NULL;
        try {
            buffer = new char[ret + 1];
        } catch (std::bad_alloc) {
            buffer = NULL;
        }
        if(buffer) {
            va_start(arglist, fmt);
            ret = ::vsnprintf(buffer, ret + 1, fmt, arglist);
            va_end(arglist);
            iptv_logging::IptvLogMessage(file, func, line, type, level, module).stream() << buffer;
            delete [] buffer;
        }
    }
    return ret;
}

