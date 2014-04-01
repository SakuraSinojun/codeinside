
#include <cstdio>
#include "exception.h"
#include <string.h>



CGPSException::CGPSException(unsigned int errvalue, const char * errdesc) :
	m_value(errvalue)
{
        strncpy(this->errdesc, errdesc, 1024);
}

unsigned int CGPSException::ErrId()
{
	return m_value;
}

const char * CGPSException::ErrDesc()
{
        return (const char *)errdesc;
}

