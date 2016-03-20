 #pragma once

#include <functional>

typedef unsigned char uchar;
typedef unsigned short ushort;
typedef unsigned int uint;
typedef unsigned long ulong;
typedef long long int64;
typedef unsigned long long uint64;

#pragma pack(push)
#pragma pack(1)

//类型转换用联合
typedef union
{
	uchar charVal[8];
	ushort shortVal[4];
	ulong longVal[2];
	float floatVal[2];
	void* ptrVal[2];
	uint64 int64Val;
	double doubleVal;
} uTypeUnion;

#pragma pack(pop)


//卡片类型
enum eCardType
{
	General = 0, 
	Base = 1,
	Scheme = 2,
	Equip = 3,
	Treasure = 4
};

//网络与主机字节顺序转换
//extern std::function<void (TYPEUNION &u, int size)> _ntoh;
//extern std::function<void (TYPEUNION &u, int size)> _hton;

void initEndianChage();