
#include "string.h"

namespace util {

std::string Trim(std::string str, const char * delimiters)
{
    size_t  pos1, pos2;
    pos1 = str.find_first_not_of(delimiters);
    pos2 = str.find_last_not_of(delimiters);
    if (pos1 == std::string::npos || pos2 == std::string::npos)
        return std::string("");
    return str.substr(pos1, pos2 - pos1 +  1);
}

} // namespace util

