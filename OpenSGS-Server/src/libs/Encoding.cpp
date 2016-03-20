#include "Encoding.h"
#include <string.h>
#include <string>

const char* UnicodeToUtf8(const wchar_t *str)
{
	char buf[256];
	wchar_t code;
	int i = 0, j = 0;

	while(str[i])
	{
		if(!(str[i] >> 7))
		{
			buf[j] = (char)str[i];
			i++, j++;
			continue;
		}
		if(str[i] >> 11)
		{
			code = str[i];
			buf[j + 2] = (char)(code & 0x003F | 0x0080);
			code = code >> 6;
			buf[j + 1] = (char)(code & 0x003F | 0x0080);
			code = code >> 6;
			buf[j] = (char)(code & 0x000F | 0x00E0);
			i++, j += 3;
			continue;
		}
		code = str[i];
		buf[j + 1] = (char)(code & 0x003F | 0x0080);
		code = code >> 6;
		buf[j] = (char)(code & 0x001F | 0x00C0);
		i++, j += 2;
		continue;
	}
	buf[j] = 0;
	return std::string(buf).c_str();
}

const wchar_t* Utf8ToUnicode(const char *str)
{
	wchar_t buf[256];
	int i = 0, j = 0;

	while(str[j])
	{
		buf[i] = 0;
		if(str[j] & 0x80)
		{
			if(str[j] & 0x40)
			{
				if(str[j] & 0x20)
				{
					buf[i] |= ((wchar_t)(str[j] & 0x0F)) << 12;
					buf[i] |= ((wchar_t)(str[j+1] & 0x3F)) << 6;
					buf[i] |= str[j+2] & 0x3F;
					i++; j += 3;
					continue;
				}
				buf[i] |= ((wchar_t)(str[j] & 0x1F)) << 11;
				buf[i] |= ((wchar_t)(str[j+1] & 0x3F)) << 5;
				i++; j += 2;
				continue;
			}
		}
		else
		{
			buf[i] = str[j];
			i++; j++;
		}
	}
	buf[i] = 0;
	return std::wstring(buf).c_str();
}