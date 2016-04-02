 #pragma once

#include <vector>
#include <algorithm>

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
	uint intVal[2];
	float floatVal[2];
	void* ptrVal[2];
	uint64 int64Val;
	double doubleVal;
} uTypeUnion;

#pragma pack(pop)

template<class Tclass>
void SuffleVector(std::vector<Tclass> vec)
{
	std::random_shuffle(vec.begin(), vec.end());
	std::random_shuffle(vec.begin(), vec.end());
}

template<class Tclass>
void SuffleList(std::list<Tclass> vec)
{
	std::random_shuffle(vec.begin(), vec.end());
	std::random_shuffle(vec.begin(), vec.end());
}