
#pragma once

#ifdef  _MSC_VER
#define _CRT_NONSTDC_NO_DEPRECATE

#define snprintf        _snprintf
#define strdup(x)       _strdup(x)
int strcasecmp(const char *s1, char *s2);

#endif

