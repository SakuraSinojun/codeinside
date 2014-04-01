
#include "packet_parser.h"
#include <cstring>
#include <string.h>
using namespace std;

std::string PacketParser::ParsePacket(const void * data, int len)
{
        static string   strBuffer;
        string          out;
        size_t          pos1, pos2;

        if(data != NULL && len != 0)
        {
                char *  temp = new char[len + 1];
                memset(temp, 0, len + 1);
                strncpy(temp, (const char *)data, len);
                strBuffer += temp;
                delete [] temp;
        }

        pos1 = strBuffer.find("<Archive>");
        if(pos1 == std::string::npos)
                return string("");

        pos2 = strBuffer.find("</Archive>", pos1);
        if(pos2 == std::string::npos)
                return string("");

        out = strBuffer.substr(pos1, pos2 - pos1 + strlen("</Archive>"));
        strBuffer = strBuffer.substr(pos2 + strlen("</Archive>"));
        return out;
}


