#include "Encoding.h"
#include "GlobalBuffer.h"

#include <string.h>
#include <string>


const char* UnicodeToUtf8(const wchar_t *str)
{
	wchar_t code;
	int i = 0, j = 0;

	while(str[i])
	{
		if(!(str[i] >> 7))
		{
			GlobalBuffer[j] = (char)str[i];
			i++, j++;
			continue;
		}
		if(str[i] >> 11)
		{
			code = str[i];
			GlobalBuffer[j + 2] = (char)(code & 0x003F | 0x0080);
			code = code >> 6;
			GlobalBuffer[j + 1] = (char)(code & 0x003F | 0x0080);
			code = code >> 6;
			GlobalBuffer[j] = (char)(code & 0x000F | 0x00E0);
			i++, j += 3;
			continue;
		}
		code = str[i];
		GlobalBuffer[j + 1] = (char)(code & 0x003F | 0x0080);
		code = code >> 6;
		GlobalBuffer[j] = (char)(code & 0x001F | 0x00C0);
		i++, j += 2;
		continue;
	}
	GlobalBuffer[j] = 0;
	return GlobalBuffer;
}

const wchar_t* Utf8ToUnicode(const char *str)
{
	int i = 0, j = 0;
	wchar_t* pbuf = (wchar_t*)GlobalBuffer;

	while(str[j])
	{
		pbuf[i] = 0;
		if(str[j] & 0x80)
		{
			if(str[j] & 0x40)
			{
				if(str[j] & 0x20)
				{
					pbuf[i] |= ((wchar_t)(str[j] & 0x0F)) << 12;
					pbuf[i] |= ((wchar_t)(str[j+1] & 0x3F)) << 6;
					pbuf[i] |= str[j+2] & 0x3F;
					i++; j += 3;
					continue;
				}
				pbuf[i] |= ((wchar_t)(str[j] & 0x1F)) << 11;
				pbuf[i] |= ((wchar_t)(str[j+1] & 0x3F)) << 5;
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