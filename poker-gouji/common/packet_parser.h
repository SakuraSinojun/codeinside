
#pragma once

#include <string>


class PacketParser
{
protected:
    // ���ڴ���ճ���ַ����� ����xml��ʽ��string��
    // data�ַ���������\0��β
    // �������������xml��������string("");
    std::string  ParsePacket(const void * data, int len = 0);
};

