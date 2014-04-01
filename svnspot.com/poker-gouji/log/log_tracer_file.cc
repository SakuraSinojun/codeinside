

#include "log_tracer_file.h"
#include "iptv_logging.h"
#include <string>
#include <stdio.h>
#include "log.h"

namespace iptv_logging {

LogTracerFile::LogTracerFile()
    : LogTracer("file")
    , filename_("log.txt")
{
}

LogTracerFile::LogTracerFile(std::string name)
    : LogTracer(name)
    , filename_("log.txt")
{
}

LogTracerFile::~LogTracerFile()
{
    ofr_.close();
}

void LogTracerFile::Output(const char* log, IptvLogMessage::RawMessage& msg)
{
    if (!ofr_)
        ofr_.open(filename_.c_str());
    if (ofr_) {
        ofr_ << log;
    }
}

}

