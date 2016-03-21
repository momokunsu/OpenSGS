#include "StringManager.h"
#include <stdarg.h>
#include "GlobalBuffer.h"

const char* StringManager::format(const char* str, ...)
{
	va_list ap;
	va_start(ap, str);
	vsnprintf(GlobalBuffer, sizeof(GlobalBuffer), str, ap);
	va_end(ap);

	return GlobalBuffer;
}

const char* StringManager::fromInt(long long n)
{
	snprintf(GlobalBuffer, sizeof(GlobalBuffer), "%lld", n);
	return GlobalBuffer;
}

const char* StringManager::fromUint(unsigned long long n)
{
	snprintf(GlobalBuffer, sizeof(GlobalBuffer), "%llu", n);
	return GlobalBuffer;
}

const char* StringManager::fromFloat(double n)
{
	snprintf(GlobalBuffer, sizeof(GlobalBuffer), "%lf", n);
	return GlobalBuffer;
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
