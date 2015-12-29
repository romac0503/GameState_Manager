#include "StdAfx.h"
#include "StringUtil.h"

#include <stdarg.h>
#include <string>

std::string StringUtil::format(const char* format, ...)
{
	char buffer[BUFFER_SIZE];
	va_list args;
	va_start(args, format);
	
#ifdef _WIN32
	vsnprintf_s(buffer, sizeof(buffer), BUFFER_SIZE, format, args);
#else
	vsnprintf(buffer, BUFFER_SIZE, format, args);
#endif
    
	va_end(args);
	return std::string(buffer);
}
