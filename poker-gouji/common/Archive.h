

#pragma once

#include <sstream>
#include <string>
#include <vector>
#include "rapidxml/rapidxml.hpp"

class CArchive
{
public:
        CArchive();
        ~CArchive();

        CArchive(const CArchive& o);
        CArchive& operator=(const CArchive& o);

        CArchive& operator<<(int i);
        CArchive& operator<<(float f);
        CArchive& operator<<(std::string str);
        CArchive& operator<<(const char* str);
        CArchive& operator<<(long l);
        CArchive& operator<<(unsigned int i);
        CArchive& operator<<(unsigned long l);
        CArchive& operator<<(bool b);

        CArchive& operator>>(int& i);
        CArchive& operator>>(float& f);
        CArchive& operator>>(std::string& str);
        CArchive& operator>>(long& l);
        CArchive& operator>>(unsigned int& i);
        CArchive& operator>>(unsigned long& l);
        CArchive& operator>>(bool& b);

        CArchive& operator<<(CArchive& o);
        CArchive& operator>>(CArchive& o);

        std::string    toStdString(void);
        CArchive& fromStdString(std::string xml);
        CArchive& fromXmlNode(rapidxml::xml_node<> * node);

        void      clear(void);
        bool      empty(void);

private:
        CArchive&       append(const char * value, const char * type);
        std::string     first(void);
        rapidxml::xml_node<> *    GetHead();

        rapidxml::xml_document<>          doc;
        std::vector<const char *>    vecStr;

        void    CopyXml(rapidxml::xml_node<> * dst, rapidxml::xml_node<> * src); 
};

std::basic_ostream<char>& operator <<(std::basic_ostream<char>& out, CArchive& ar);






