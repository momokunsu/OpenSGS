#include "CmdBuffer.h"

#pragma pack(push)
#pragma pack(1)

typedef struct
{
	ushort cmd;
	uchar datalen;
	char data[1];
}COMMOND;

#pragma pack(pop)

//获取指令数据指针
#define GETCMDDATAPTR(cmd) ((void *)(((COMMOND*)cmd)->data))
//获取指令大小
#define GETCMDSIZE(cmd) (sizeof(COMMOND)+((COMMOND*)cmd)->datalen-1)

CmdBuffer::CmdBuffer(void)
{
	init();
	initEndianChage();
}

CmdBuffer::~CmdBuffer(void)
{
}

void CmdBuffer::init()
{
	memset(_buffer, 0, sizeof(_buffer));
	resetPos();
	_wpos = _buffer;
}

int CmdBuffer::getPos() const
{
	return (char*)_pos - (char*)GETCMDDATAPTR(_buffer);
}

void CmdBuffer::setPos(int pos)
{
	_pos = (char*)GETCMDDATAPTR(_buffer) + pos;
}

int CmdBuffer::getBufSize() const
{
	return GETCMDSIZE(_buffer);
}



void CmdBuffer::resetPos()
{
	_pos = GETCMDDATAPTR(_buffer);
}

ushort CmdBuffer::getCmdCode() const
{
	TYPEUNION u;
	u.shortVal[0] = ((COMMOND *)_buffer)->cmd;
	_ntoh(u, sizeof(ushort));
	return u.shortVal[0];
}

int CmdBuffer::recvData(void *data, int len)
{
	if(len > 0)
	{
		char *pdata = (char *)data;
		for(int i = 0; i < len; i++)
		{
			((char*)_wpos)[i] = pdata[i];
		}
		_wpos = ((char*)_wpos)+len;
	}
	if(getCmdCode())
		return ((char*)_wpos) - _buffer - GETCMDSIZE(_buffer);
	return 0;
}

void CmdBuffer::setCmdCode(ushort cmd)
{
	TYPEUNION u;
	u.shortVal[0] = cmd;
	_hton(u, sizeof(ushort));
	((COMMOND *)_buffer)->cmd = u.shortVal[0];
}

int CmdBuffer::getDataLen() const
{
	return ((COMMOND *)_buffer)->datalen;
}
void CmdBuffer::setDataLenForPos()
{
	((COMMOND *)_buffer)->datalen = ((char*)_pos) - ((char*)GETCMDDATAPTR(_buffer));
}

bool CmdBuffer::canHandle() const
{
	if(((char*)_pos)-_buffer > (int)GETCMDSIZE(_buffer))
		return true;
	else
		return false;
}

void CmdBuffer::copyBuffer(CmdBuffer *buffer)
{
	memcpy(_buffer, buffer->_buffer, sizeof(_buffer));
	resetPos();
	_wpos = buffer->_wpos;
}


uchar CmdBuffer::readByte()
{
	TYPEUNION *ret = (TYPEUNION *)_pos;
	_pos = ((char*)_pos) + 1;
	return ret->charVal[0];
}
ushort CmdBuffer::readWord()
{
	TYPEUNION *ret = (TYPEUNION *)_pos;
	_ntoh(*ret, sizeof(short));
	_pos = ((short*)_pos) + 1;
	return ret->shortVal[0];
}
ulong CmdBuffer::readDword()
{
	TYPEUNION *ret = (TYPEUNION *)_pos;
	_ntoh(*ret, sizeof(long));
	_pos = ((long*)_pos) + 1;
	return ret->longVal[0];
}
uint64 CmdBuffer::readDDword()
{
	TYPEUNION *ret = (TYPEUNION *)_pos;
	_ntoh(*ret, sizeof(long long));
	_pos = ((long long*)_pos) + 1;
	return ret->int64Val;
}
float CmdBuffer::readFloat()
{
	TYPEUNION *ret = (TYPEUNION *)_pos;
	_ntoh(*ret, sizeof(float));
	_pos = ((float*)_pos) + 1;
	return ret->floatVal[0];
}
double CmdBuffer::readDouble()
{
	TYPEUNION *ret = (TYPEUNION *)_pos;
	_ntoh(*ret, sizeof(double));
	_pos = ((double*)_pos) + 1;
	return ret->doubleVal;
}
const char *CmdBuffer::readText()
{
	const char *ret;
	char *tmp;
	ret = tmp = (char *)_pos;
	while(*tmp)
		tmp++;
	_pos = tmp+1;
	return ret;
}
void CmdBuffer::readData(void *data, int len)
{
	for(int i = 0; i < len; i++)
		((char *)data)[i] = ((char *)_pos)[i];
	_pos = ((char*)_pos) + len;
}


void CmdBuffer::writeByte(uchar c)
{
	TYPEUNION *ret = (TYPEUNION *)_pos;
	_pos = ((char*)_pos) + 1;
	ret->charVal[0] = c;
}
void CmdBuffer::writeWord(ushort s)
{
	TYPEUNION *ret = (TYPEUNION *)_pos;
	_pos = ((short*)_pos) + 1;
	ret->shortVal[0] = s;
	_hton(*ret, sizeof(short));
}
void CmdBuffer::writeDword(ulong l)
{
	TYPEUNION *ret = (TYPEUNION *)_pos;
	_pos = ((long*)_pos) + 1;
	ret->longVal[0] = l;
	_hton(*ret, sizeof(long));
}
void CmdBuffer::writeDDword(uint64 ll)
{
	TYPEUNION *ret = (TYPEUNION *)_pos;
	_pos = ((long long*)_pos) + 1;
	ret->int64Val = ll;
	_hton(*ret, sizeof(long long));
}
void CmdBuffer::writeFloat(float f)
{
	TYPEUNION *ret = (TYPEUNION *)_pos;
	_pos = ((float*)_pos) + 1;
	ret->floatVal[0] = f;
	_hton(*ret, sizeof(float));
}
void CmdBuffer::writeDouble(double d)
{
	TYPEUNION *ret = (TYPEUNION *)_pos;
	_pos = ((double*)_pos) + 1;
	ret->doubleVal = d;
	_hton(*ret, sizeof(double));
}
int CmdBuffer::writeText(const char *str)
{
	while(*str)
	{
		*((char *)_pos) = *str;
		str++;
		_pos = ((char *)_pos)+1;
	}
	*((char *)_pos) = 0;
	_pos = ((char *)_pos)+1;
	return strlen(str);
}
void CmdBuffer::writeData(void *data, int len)
{
	for(int i = 0; i < len; i++)
	{
		*((char *)_pos) = *((char *)data);
		data = ((char *)data)+1;
		_pos = ((char *)_pos)+1;
	}
}