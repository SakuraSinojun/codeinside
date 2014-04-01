
#pragma once

#include <string>


class PacketParser
{
protected:
    // 用于处理粘连字符串。 返回xml样式的string。
    // data字符串比须以\0结尾
    // 如果不存在完整xml串，返回string("");
    std::string  ParsePacket(const void * data, int len = 0);
};

