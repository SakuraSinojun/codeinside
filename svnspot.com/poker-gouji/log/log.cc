

#include "log.h"
#include "log_tracer_shell.h"
#include "log_tracer_file.h"

using namespace iptv_logging;

void InitLog(void)
{
    IptvLog::GetInstance()->AddTracer(new LogTracerShell());
//    IptvLog::GetInstance()->AddTracer(new LogTracerFile());
}

