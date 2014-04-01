

#ifndef __LOG_LOG_TRACER_SHELL_H__
#define __LOG_LOG_TRACER_SHELL_H__
#pragma once

#include "log_tracer.h"

namespace iptv_logging {

class LogTracerShell : public LogTracer
{
public:
    LogTracerShell();
    LogTracerShell(std::string name);
    virtual ~LogTracerShell();
    virtual void Output(const char * log, IptvLogMessage::RawMessage& msg);
};
}

#endif

