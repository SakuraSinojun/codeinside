

#ifndef __LOG_LOG_TRACER_H__
#define __LOG_LOG_TRACER_H__
#pragma once

#include <string>
#include <vector>
#include "iptv_logging.h"

namespace iptv_logging {

class IptvLog;

// Component:
class LogTracer {
public:
    LogTracer(){name_ = "";}
    LogTracer(std::string name){name_ = name;}
    virtual ~LogTracer(){};
    virtual void Output(const char * log, IptvLogMessage::RawMessage& msg) = 0;
    std::string get_name_(){return name_;}
protected:
    std::string name_;
private:
    friend class IptvLog;
    // ever-traced flag for IptvLog.
    bool Logged(void) {return hasLogged;}
    void SetLogged(bool s = true) {hasLogged = s;}
    bool hasLogged;
};

typedef std::vector<LogTracer *>              LogTracerContainer;
typedef std::vector<LogTracer *>::iterator    LogTracerIterator;

// Decorator:
// won't free memory.
// you should free the tracers by yourself.
class IptvLog
{
public:
    ~IptvLog();

    static IptvLog * GetInstance();
    void AddTracer(LogTracer* rec);
    void RemoveTracer(LogTracer* rec);
    LogTracer* RemoveTracer(std::string recname);

    // Send logs to the LogTracers. It won't be used as recursion.
    // Check tls_logged_ for details.
    void Output(const char * log, IptvLogMessage::RawMessage& msg);

    static std::string GetLastErrorMessage(void);

private:
    IptvLog();
    LogTracerContainer  rec_list_;
};

}   // namespace iptv_logging
#endif


