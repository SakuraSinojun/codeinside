#ifndef __LOG_LOG_H__
#define __LOG_LOG_H__
#pragma once


// 
// 日志输出模块
// ------------------------------------------
//
// 提供了一系列输出日志的宏。目前输出日志的方法是将要输出的内容按流的方式扔给
// SLOG(<type as syslog>, <level as syslog>)宏。例如：
//
//      SLOG(USER, DEBUG) << "this is user + debug message.";
//
// 也可以使用条件判断来决定是否输出日志，例如：
//
//      SLOG_IF(USER, DEBUG, num > 3) << "num is bigger.";
//
// 这样的调用会将输出消息封装后交给相应的处理模块，比如：串口打印、24端口输出、
// 华为syslog规范定义的 UDP/FTP 方式上报等。
//
// SCHECK(condition)宏在 debug 与 release 的情形下都会生效，它的作用有一些类似
// assert()，条件为假时会调用SLOG(FATAL, DEBUG)以引发 crashdump。当调试器存在
// 时，这个行为会通知调试器中断程序进入调试模式；当调试器不存在时，这个行为会
// 触发一个断点并中止程序运行。例如：
//
//      SCHECK(str != NULL) << "Will break debugger if str is null";
//
// 相应地，有一套以'D'开头的宏，只会在发布为 debug 时生效，在 release 的情形下，
// 它们将什么也不做。例如：
//
//      DSLOG(USER, DEBUG) << "Will output only in debug build.";
//      DSCHECK(p != NULL) << "Will break debugger only in debug build.";
//
// 另外有一套以'V'头头的宏，只会在增加运行参数 -verbose 时进行日志输出。（到写
// 这个说明的时候为止，这套宏的行为跟SLOG()一致，只存在计划中。但是请充分使用它
// 而不是SLOG()以区分 有效日志 与 调试日志。
//
// 使用SLOG()等宏时，需要提供当前文件所在的模块名字，请在.cc/.cpp文件中使用
// DECLARE_MODULE(name)宏来声明当前文件所属模块名。例如：
// 
//      // file: upgrader.cc
//      #include "log.h"
//      DECLARE_MODULE(upgrader);
//      ...
//
//
// 目前已经提供的日志宏列表与功能如下：
//
//  SLOG(TYPE, LEVEL)       以流的方式输出日志，TYPE与LEVEL遵守华为syslog规范。
//  SPLOG(message)          在SLOG()的基础上输出系统错误消息。类似perror()。
//  SLOG_IF(TYPE, LEVEL, condition)     当condition为真时输出日志。
//  SCHECK(condition)       断言condition为真。否则中止程序或激活调试器。
//  DSLOG(TYPE, LEVEL)      在未定义NDEBUG的时候同SLOG，否则什么也不做。
//  DSCHECK(contition)      在未定义NDEBUG的时候同SCHECK，否则什么也不做。
//  RUN_HERE()              以USER+DEBUG等级打印Run here消息。NDEBUG时什么也不做。
//  ScopeTrace(x)           以x为名字的作用域跟踪。进入和离开作用域会打印不同的提示。
//



// LogType与LogLevel的定义。
#include "iptv_logging.h"

// 直接按syslog规范来定义常量。
#define LOGTYPE_OPERATION   16
#define LOGTYPE_RUNNING     17
#define LOGTYPE_SECURITY    19
#define LOGTYPE_USER        20
#define LOGTYPE_FATAL       30

#define LOGLEVEL_LERROR      3
#define LOGLEVEL_INFORMATION 6
#define LOGLEVEL_DEBUG       7

// 在log终端中输出颜色。目前也用了NDEBUG来决定开关。
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
// 例： SLOG(USER, DEBUG) << "User debug message.";
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
// 用了assert.h中的NDEBUG宏。
// D**宏表示只在DEBUG版本会打印输出。
//
#ifndef NDEBUG

// DSLOG: 只在Debug版本会输出内容。
#define DSLOG(type, level)  SLOG_IF(type, level, true)

// DSCHECK: 在Debug版本中会中断程序运行，行为类似assert()， 在release版本中仅是
// 一个错误打印。
#define DSCHECK(condition)  SLOG_IF(FATAL, LERROR, !(condition)) << "Check failed: " << #condition ". "

// RUN_HERE: 在release版本中不起任何作用。
#define RUN_HERE()          SLOG(USER, DEBUG) << TERMC_YELLOW << "Run here! " << TERMC_NONE

// ScopeTrace(x): 以x为名字的作用域跟踪。 进入和离开作用域会打印不同的提示。
// 在release版本中不起任何作用。
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


// 声明当前所属模块名，以供构造IptvLogMessage()类的宏调用。
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

