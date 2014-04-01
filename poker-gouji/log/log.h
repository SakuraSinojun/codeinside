#ifndef __LOG_LOG_H__
#define __LOG_LOG_H__
#pragma once


// 
// ��־���ģ��
// ------------------------------------------
//
// �ṩ��һϵ�������־�ĺꡣĿǰ�����־�ķ����ǽ�Ҫ��������ݰ����ķ�ʽ�Ӹ�
// SLOG(<type as syslog>, <level as syslog>)�ꡣ���磺
//
//      SLOG(USER, DEBUG) << "this is user + debug message.";
//
// Ҳ����ʹ�������ж��������Ƿ������־�����磺
//
//      SLOG_IF(USER, DEBUG, num > 3) << "num is bigger.";
//
// �����ĵ��ûὫ�����Ϣ��װ�󽻸���Ӧ�Ĵ���ģ�飬���磺���ڴ�ӡ��24�˿������
// ��Ϊsyslog�淶����� UDP/FTP ��ʽ�ϱ��ȡ�
//
// SCHECK(condition)���� debug �� release �������¶�����Ч������������һЩ����
// assert()������Ϊ��ʱ�����SLOG(FATAL, DEBUG)������ crashdump��������������
// ʱ�������Ϊ��֪ͨ�������жϳ���������ģʽ����������������ʱ�������Ϊ��
// ����һ���ϵ㲢��ֹ�������С����磺
//
//      SCHECK(str != NULL) << "Will break debugger if str is null";
//
// ��Ӧ�أ���һ����'D'��ͷ�ĺֻ꣬���ڷ���Ϊ debug ʱ��Ч���� release �������£�
// ���ǽ�ʲôҲ���������磺
//
//      DSLOG(USER, DEBUG) << "Will output only in debug build.";
//      DSCHECK(p != NULL) << "Will break debugger only in debug build.";
//
// ������һ����'V'ͷͷ�ĺֻ꣬�����������в��� -verbose ʱ������־���������д
// ���˵����ʱ��Ϊֹ�����׺����Ϊ��SLOG()һ�£�ֻ���ڼƻ��С���������ʹ����
// ������SLOG()������ ��Ч��־ �� ������־��
//
// ʹ��SLOG()�Ⱥ�ʱ����Ҫ�ṩ��ǰ�ļ����ڵ�ģ�����֣�����.cc/.cpp�ļ���ʹ��
// DECLARE_MODULE(name)����������ǰ�ļ�����ģ���������磺
// 
//      // file: upgrader.cc
//      #include "log.h"
//      DECLARE_MODULE(upgrader);
//      ...
//
//
// Ŀǰ�Ѿ��ṩ����־���б��빦�����£�
//
//  SLOG(TYPE, LEVEL)       �����ķ�ʽ�����־��TYPE��LEVEL���ػ�Ϊsyslog�淶��
//  SPLOG(message)          ��SLOG()�Ļ��������ϵͳ������Ϣ������perror()��
//  SLOG_IF(TYPE, LEVEL, condition)     ��conditionΪ��ʱ�����־��
//  SCHECK(condition)       ����conditionΪ�档������ֹ����򼤻��������
//  DSLOG(TYPE, LEVEL)      ��δ����NDEBUG��ʱ��ͬSLOG������ʲôҲ������
//  DSCHECK(contition)      ��δ����NDEBUG��ʱ��ͬSCHECK������ʲôҲ������
//  RUN_HERE()              ��USER+DEBUG�ȼ���ӡRun here��Ϣ��NDEBUGʱʲôҲ������
//  ScopeTrace(x)           ��xΪ���ֵ���������١�������뿪��������ӡ��ͬ����ʾ��
//



// LogType��LogLevel�Ķ��塣
#include "iptv_logging.h"

// ֱ�Ӱ�syslog�淶�����峣����
#define LOGTYPE_OPERATION   16
#define LOGTYPE_RUNNING     17
#define LOGTYPE_SECURITY    19
#define LOGTYPE_USER        20
#define LOGTYPE_FATAL       30

#define LOGLEVEL_LERROR      3
#define LOGLEVEL_INFORMATION 6
#define LOGLEVEL_DEBUG       7

// ��log�ն��������ɫ��ĿǰҲ����NDEBUG���������ء�
#ifndef NDEBUG
#define TERMC_RED     "\033[1;31;40m"
#define TERMC_GREEN   "\033[1;32;40m"
#define TERMC_YELLOW  "\033[1;33;40m"
#define TERMC_BLUE    "\033[1;34;40m"
#define TERMC_PINK    "\033[1;35;40m"
#define TERMC_NONE    "\033[m"
#else
#define TERMC_RED     ""
#define TERMC_GREEN   ""
#define TERMC_YELLOW  ""
#define TERMC_BLUE    ""
#define TERMC_PINK    ""
#define TERMC_NONE    ""
#endif

#ifdef _MSC_VER
#define __func__ __FUNCTION__
#endif

#include "log_tracer.h"

//
// ���� SLOG(USER, DEBUG) << "User debug message.";
//      SLOG(RUNNING, INFORMATION) << "Running infomation message.";
//
#define SLOG(type, level)    \
    if(true) \
        iptv_logging::IptvLogMessage(__FILE__, __func__, __LINE__, \
            LOGTYPE_ ## type, LOGLEVEL_ ## level, \
            IptvLogGetCurrentModule()).stream()

#define SPLOG(message)  \
    SLOG(RUNNING, LERROR) << message << ":" << iptv_logging::IptvLog::GetLastErrorMessage() << " "

#define SLOG_IF(type, level, condition)  \
    if (condition) SLOG(type, level)

#define SCHECK(condition)    \
    SLOG_IF(FATAL, LERROR, !(condition))


#define LOG()       SLOG(USER, INFORMATION)
#define Debug()     SLOG(USER, DEBUG)

#define VSLOG       SLOG
#define VSPLOG      SPLOG
#define VSLOG_IF    SLOG_IF

//
// ����assert.h�е�NDEBUG�ꡣ
// D**���ʾֻ��DEBUG�汾���ӡ�����
//
#ifndef NDEBUG

// DSLOG: ֻ��Debug�汾��������ݡ�
#define DSLOG(type, level)  SLOG_IF(type, level, true)

// DSCHECK: ��Debug�汾�л��жϳ������У���Ϊ����assert()�� ��release�汾�н���
// һ�������ӡ��
#define DSCHECK(condition)  SLOG_IF(FATAL, LERROR, !(condition)) << "Check failed: " << #condition ". "

// RUN_HERE: ��release�汾�в����κ����á�
#define RUN_HERE()          SLOG(USER, DEBUG) << TERMC_YELLOW << "Run here! " << TERMC_NONE

// ScopeTrace(x): ��xΪ���ֵ���������١� ������뿪��������ӡ��ͬ����ʾ��
// ��release�汾�в����κ����á�
class ScopeTracer {
public:
    ScopeTracer(const char * func, const char * name, int line) : name_(name), func_(func), line_(line) {
        iptv_logging::IptvLogMessage().stream() << TERMC_RED << "Into scope: " << name << " (" << func << ":" << line << ")" << TERMC_NONE; 
    }
    ~ScopeTracer() { 
        iptv_logging::IptvLogMessage().stream() << TERMC_GREEN << "Leave scope: " << name_ << " (" << func_ << ") from line: " << line_ << TERMC_NONE; 
    }
private:
    const char * IptvLogGetCurrentModule(void){return name_;}
    const char * name_;
    const char * func_;
    int          line_;
};
#define ScopeTrace(x)       ScopeTracer x ## __LINE__(__func__, #x, __LINE__)

#else    // ifndef NDEBUG

#define DSLOG(type, level)  SLOG_IF(type, level, false)
#define DSCHECK(condition)  SLOG_IF(RUNNING, LERROR, !(condition)) << "Check failed: " << #condition ". "
#define RUN_HERE()          SLOG_IF(USER, DEBUG, false)
#define ScopeTrace(x)       ((void)0);

#endif // ifndef NDEBUG


// ������ǰ����ģ�������Թ�����IptvLogMessage()��ĺ���á�
#define DECLARE_MODULE(x)   \
    static iptv_logging::ModuleListHelper g_current_module_name_of_this_scope(#x); \
    static const char * IptvLogGetCurrentModule(void){return #x;}


int log_trace(const char * file,
    const char * func,
    int line,
    IptvLogType type,
    IptvLogLevel level,
    const char * module,
    const char * fmt, ...);

#define PRINTF(X, ...) \
    log_trace(__FILE__, __func__, __LINE__, LOGTYPE_USER, LOGLEVEL_DEBUG, IptvLogGetCurrentModule(), X, ##__VA_ARGS__)


void InitLog(void);

#endif //__LOG_LOG_H__

