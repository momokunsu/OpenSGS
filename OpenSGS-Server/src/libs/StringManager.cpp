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

const char* StringManager::trimBegin(const char* str, ...)
{
	va_list ap;
	va_start(ap, str);
	char val = va_arg(ap, char);
	vector<char> vec;
	while (val)
		vec.push_back(val);
	const char* pbuf = trimBegin(str, vec);
	va_end(ap);

	return pbuf;
}

const char* StringManager::trimBegin(const char* str, const std::vector<char> &vec)
{
	char* pbuf = (char*)GC::getGlobalBuffer();
	string strobj;
	if (GC::isRangeOfGlobalBuf(str))
	{
		strobj = str;
		str = strobj.c_str();
	}
}

const char* StringManager::trimEnd(const char* str, ...)
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

const char* StringManager::trimEnd(const char* str, const std::vector<char> &vec)
{

}

bool StringManager::isContainChar(const char* str, ...)
{
	va_list ap;
	va_start(ap, str);
	char val = va_arg(ap, char);
	while (val)
	{
		int i = 0;
		while (str[i])
		{
			if (str[i] == val)
			{
				va_end(ap);
				return true;
			}
			i++;
		}
		val = va_arg(ap, char);
	}
	va_end(ap);
	return false;
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
