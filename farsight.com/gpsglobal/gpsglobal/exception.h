

#pragma once

#ifndef __EXCEPTION_H__
#define __EXCEPTION_H__


enum
{
	ERR_SUCCESS = 0,
        ERR_OPENVIDEO,
	
};

class CGPSException
{
public:
	CGPSException(unsigned int errvalue, const char * errdesc);
	unsigned int ErrId();
        const char * ErrDesc();
private:
	unsigned int    m_value;
        char            errdesc[1024];

};


#endif

