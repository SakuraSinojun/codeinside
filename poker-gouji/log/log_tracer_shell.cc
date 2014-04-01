

#include "log_tracer_shell.h"
#include "iptv_logging.h"
#include <string>
#include <stdio.h>
#include "log.h"

namespace iptv_logging {

LogTracerShell::LogTracerShell() :
    LogTracer()
{
    name_ = "shell";
}

LogTracerShell::LogTracerShell(std::string name) :
    LogTracer(name)
{
}

LogTracerShell::~LogTracerShell()
{
}

void LogTracerShell::Output(const char* log, IptvLogMessage::RawMessage& msg)
{
    ::fprintf(stderr, "%s", log);
    fflush(stderr);
}

}

