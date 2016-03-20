#include "StringManager.h"
#include <stdarg.h>

#define BUFFER_SIZE (1024 * 1024)

std::string StringManager::formatString(const char* str, ...)
{
	va_list ap;
	va_start(ap, str);

	std::string ret = "";
	char *buf = (char*)malloc(BUFFER_SIZE);
	if (buf)
	{
		vsnprintf(buf, BUFFER_SIZE, str, ap);
		ret = std::string(buf);
		free(buf);
	}
	va_end(ap);

	return ret;
}

const char* StringManager::format(const char* str, ...)
{
	va_list ap;
	va_start(ap, str);

	const char* ret = nullptr;
	char *buf = (char*)malloc(BUFFER_SIZE);
	if(buf)
	{
		vsnprintf(buf, BUFFER_SIZE, str, ap);
		ret = std::string(buf).c_str();
		free(buf);
	}
	va_end(ap);

	return ret;
}

std::string StringManager::stringFromInt(long long n)
{
	return std::to_string(n);
}

std::string StringManager::stringFromUint(unsigned long long n)
{
	return std::to_string(n);
}

std::string StringManager::stringFromFloat(double n)
{
	return std::to_string(n);
}

long long StringManager::toInt(const char* str)
{
	return std::stoll(str);
}

unsigned long long StringManager::toUint(const char* str)
{
	return std::stoull(str);
}

double StringManager::toFloat(const char* str)
{
	return std::stod(str);
}
