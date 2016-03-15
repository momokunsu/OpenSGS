#include "def.h"

std::function<void (TYPEUNION &u, int size)> _ntoh = nullptr;
std::function<void (TYPEUNION &u, int size)> _hton = nullptr;

void endianChage(TYPEUNION &u, int size)
{
	int dsize = size / 2;
	for(int i = 0; i < dsize; i++)
	{
		uchar t = u.charVal[i];
		u.charVal[i] = u.charVal[size-i-1];
		u.charVal[size-i-1] = t;
	}
}

void endianNotChage(TYPEUNION &u, int size)
{
}

void initEndianChage()
{
	TYPEUNION u;
	u.longVal[0] = 0xFF0000FF;
	if(u.shortVal[0] == 0x00FF && u.shortVal[1] == 0xFF00)
		_hton = _ntoh = endianChage;
	else
		_hton = _ntoh = endianNotChage;
}