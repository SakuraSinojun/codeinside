

#pragma once

#include "log_tracer.h"

#include <string>
#include <fstream>

namespace iptv_logging {

class LogTracerFile : public LogTracer
{
public:
    LogTracerFile();
    LogTracerFile(std::string name);
    virtual ~LogTracerFile();
    virtual void Output(const char * log, IptvLogMessage::RawMessage& msg);
private:
    std::string filename_;
    std::ofstream   ofr_;
};
}


