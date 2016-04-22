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

const char* StringManager::replace(const char* str, char src, char dst)
{
	char* pbuf = (char*)GC::getGlobalBuffer();
	string strobj;
	if (GC::isRangeOfGlobalBuf(str))
	{
		strobj = str;
		str = strobj.c_str();
	}

	auto ret = pbuf;
	int index = indexOf(str, src);
	while (index != -1)
	{
		memcpy(pbuf, str, index + 1);
		pbuf += index, str += index;

		*pbuf = dst, str++;
		if(dst) pbuf ++;

		index = indexOf(str, src);
	}
	strcpy(pbuf, str);

	return ret;
}

const char* StringManager::replace(const char* str, const char* src, const char* dst)
{
	char* pbuf = (char*)GC::getGlobalBuffer();
	string strobj;
	if (GC::isRangeOfGlobalBuf(str))
	{
		strobj = str;
		str = strobj.c_str();
	}

	auto ret = pbuf;
	int srcsize = strlen(src);
	int dstsize = strlen(dst);
	int index = indexOf(str, src);
	while (index != -1)
	{
		memcpy(pbuf, str, index + 1);
		pbuf += index, str += index;

		strcpy(pbuf, dst);
		str += srcsize, pbuf += dstsize;

		index = indexOf(str, src);
	}
	strcpy(pbuf, str);

	return ret;
}

const void StringManager::split(const char* str, std::vector<std::string>& arr, va_list ap)
{
	char* pbuf = (char*)GC::getGlobalBuffer();
	strcpy(pbuf, str);

	while (*pbuf)
	{
		if (isContainsChar(*pbuf, ap))
		{
			pbuf++;
			continue;
		}

		auto it = pbuf;
		while (*it && !isContainsChar(*it, ap)) it++;
		if(*it) *it = 0, it++;
		arr.push_back(pbuf);
		pbuf = it;
	}
}

const void StringManager::split(const char* str, std::vector<std::string>& arr, ...)
{
	va_list ap;
	va_start(ap, str);
	split(str, arr, ap);
	va_end(ap);
}

int StringManager::indexOf(const char* str, const char* dst)
{
	int i = 0, j = 0;

	if (!dst[j])
		return -1;

	while (str[i])
	{
		while (dst[j])
		{
			if (str[i + j] != dst[j])
				break;
			 j++;
		}
		if (!dst[j])
			return i;
		else
			j = 0;
		i++;
	}
	return -1;
}

int StringManager::indexOf(const char* str, char dst)
{
	int i = 0;

	if (!dst)
		return -1;

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

void StringManager::toFormatValue(const char* str, const char* fmt, void *value)
{
	sscanf(str, fmt, value);
}

const char* StringManager::UTF16LToUTF8(const wchar_t *str)
{
	wchar_t code;
	int i = 0, j = 0;
	char* pbuf = (char*)GC::getGlobalBuffer();

	while (str[i])
	{
		if (!(str[i] >> 7))
		{
			pbuf[j] = (char)str[i];
			i++, j++;
			continue;
		}
		if (str[i] >> 11)
		{
			code = str[i];
			pbuf[j + 2] = (char)(code & 0x003F | 0x0080);
			code = code >> 6;
			pbuf[j + 1] = (char)(code & 0x003F | 0x0080);
			code = code >> 6;
			pbuf[j] = (char)(code & 0x000F | 0x00E0);
			i++, j += 3;
			continue;
		}
		code = str[i];
		pbuf[j + 1] = (char)(code & 0x003F | 0x0080);
		code = code >> 6;
		pbuf[j] = (char)(code & 0x001F | 0x00C0);
		i++, j += 2;
		continue;
	}
	pbuf[j] = 0;
	return pbuf;
}

const wchar_t* StringManager::UTF8ToUTF16L(const char *str)
{
	int i = 0, j = 0;
	wchar_t* pbuf = (wchar_t*)GC::getGlobalBuffer();

	while (str[j])
	{
		pbuf[i] = 0;
		if (str[j] & 0x80)
		{
			if (str[j] & 0x40)
			{
				if (str[j] & 0x20)
				{
					pbuf[i] |= ((wchar_t)(str[j] & 0x0F)) << 12;
					pbuf[i] |= ((wchar_t)(str[j + 1] & 0x3F)) << 6;
					pbuf[i] |= str[j + 2] & 0x3F;
					i++; j += 3;
					continue;
				}
				pbuf[i] |= ((wchar_t)(str[j] & 0x1F)) << 11;
				pbuf[i] |= ((wchar_t)(str[j + 1] & 0x3F)) << 5;
				i++; j += 2;
				continue;
			}
		}
		else
		{
			pbuf[i] = str[j];
			i++; j++;
		}
	}
	pbuf[i] = 0;
	return pbuf;
}