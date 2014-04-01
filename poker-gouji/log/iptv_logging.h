
#ifndef __LOG_IPTV_LOGGING_H__
#define __LOG_IPTV_LOGGING_H__
#pragma once

typedef int     IptvLogType;
typedef int     IptvLogLevel;

#ifdef __cplusplus

#include <sstream>
#include <string>

namespace iptv_logging {

class IptvLogMessage {
public:
    IptvLogMessage(const char * file, const char * func, int line,
        IptvLogType type, IptvLogType level, const char * module = "");

    IptvLogMessage(const char * file, const char * func, int line, const char * module = "");
    IptvLogMessage();

    // format the logs and output.
    virtual ~IptvLogMessage();
    std::ostream&   stream() { return stream_; }

    class RawMessage{
    public:
        RawMessage(const char * file, const char * func, int line, IptvLogType type, IptvLogLevel leve, const char * module = "");
        std::string     file_;
        std::string     func_;
        int             line_;
        IptvLogType     type_;
        IptvLogLevel    level_;
        std::string     module_;
    };
private:
    IptvLogMessage(const IptvLogMessage& o);
    IptvLogMessage& operator=(const IptvLogMessage& o);

    std::ostringstream  stream_;

    RawMessage          raw_message_;
};

class ModuleListHelper {
public:
    ModuleListHelper(const char * name);
    static void OutputAllModules(void);
private:
    const char * module_name_;
};

} // namespace iptv_logging

#endif      // ifdef __cplusplus
#endif      // __LOG_LOG_IPTV_LOGGING_H__


