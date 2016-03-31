#include "StringManager.h"
#include "GC.h"

#include <stdarg.h>

using namespace std;

const char* StringManager::format(const char* str, ...)
{
	char* pbuf = (char*)GC::getGlobalBuffer();
	string strobj;
	if (GC::isRangeOfGlobalBuf(str))
	{
		strobj = str;
		str = strobj.c_str();
	}

	va_list ap;
	va_start(ap, str);
	vsnprintf(pbuf, GC::getGlobalBufSize(), str, ap);
	va_end(ap);

	return pbuf;
}

const char* StringManager::trimBegin(const char* str, va_list ap)
{
	char* pbuf = (char*)GC::getGlobalBuffer();
	string strobj;
	if (GC::isRangeOfGlobalBuf(str))
	{
		strobj = str;
		str = strobj.c_str();
	}

	while (str[0] && isContainsChar(str[0], ap))
		str++;
	strcpy(pbuf, str);

	return pbuf;
}

const char* StringManager::trimBegin(const char* str, ...)
{
	va_list ap;
	va_start(ap, str);
	auto pbuf = trimBegin(str, ap);
	va_end(ap);

	return pbuf;
}

const char* StringManager::trimEnd(const char* str, va_list ap)
{
	char* pbuf = (char*)GC::getGlobalBuffer();
	string strobj;
	if (GC::isRangeOfGlobalBuf(str))
	{
		strobj = str;
		str = strobj.c_str();
	}

	strcpy(pbuf, str);
	int n = strlen(pbuf) - 1;
	for (int i = n; i > -1; i--)
	{
		if (!isContainsChar(str[i], ap))
			break;
		pbuf[i] = 0;
	}

	return pbuf;
}

const char* StringManager::trimEnd(const char* str, ...)
{
	va_list ap;
	va_start(ap, str);
	auto pbuf = trimEnd(str, ap);
	va_end(ap);

	return pbuf;
}

const char* StringManager::replace(const char* str, const char* src, const char* dst)
{

}

int StringManager::indexOf(const char* str, const char* dst)
{
	int i = 0, j = 0;
	while (str[i])
	{
		while (dst[j])
		{
			if (str[i] != dst[j])
				break;
			j++;
		}
		if (!dst[j])	return i;
		i++;
	}
	return -1;
}

int StringManager::indexOf(const char* str, char dst)
{
	int i = 0;
	while (str[i])
	{
		if (str[i] == dst)	return i;
		i++;
	}
	return -1;
}

bool StringManager::isContainsChar(const char* str, va_list ap)
{
	int i = 0;
	while (str[i])
	{
		if(isContainsChar(str[i], ap))
			return true;
		i++;
	}
	return false;
}

bool StringManager::isContainsChar(const char* str, ...)
{
	va_list ap;
	va_start(ap, str);
	auto pbuf = isContainsChar(str, ap);
	va_end(ap);

	return pbuf;
}

bool StringManager::isContainsChar(char cvar, va_list ap)
{
	char val = va_arg(ap, char);
	while (val)
	{
		if (cvar == val)
			return true;
		val = va_arg(ap, char);
	}
	return false;
}

bool StringManager::isContainsChar(char cvar, ...)
{
	va_list ap;
	va_start(ap, cvar);
	auto pbuf = isContainsChar(cvar, ap);
	va_end(ap);

	return pbuf;
}

const char* StringManager::fromInt(long long n)
{
	char* pbuf = (char*)GC::getGlobalBuffer();
	string strobj;
	snprintf(pbuf, GC::getGlobalBufSize(), "%lld", n);
	return pbuf;
}

const char* StringManager::fromUint(unsigned long long n)
{
	char* pbuf = (char*)GC::getGlobalBuffer();
	string strobj;
	snprintf(pbuf, GC::getGlobalBufSize(), "%llu", n);
	return pbuf;
}

const char* StringManager::fromFloat(double n)
{
	char* pbuf = (char*)GC::getGlobalBuffer();
	snprintf(pbuf, GC::getGlobalBufSize(), "%lf", n);
	return pbuf;
}

long long StringManager::toInt(const char* str)
{
	return stoll(str);
}

unsigned long long StringManager::toUint(const char* str)
{
	return stoull(str);
}

double StringManager::toFloat(const char* str)
{
	return stod(str);
}
