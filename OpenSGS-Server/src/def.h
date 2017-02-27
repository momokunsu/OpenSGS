 #pragma once

#include <vector>
#include <list>
#include <algorithm>
#include <stdlib.h>

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
void SuffleVector(std::vector<Tclass> &vec)
{
	std::random_shuffle(vec.begin(), vec.end());
	std::random_shuffle(vec.begin(), vec.end());
}

template<class Tclass>
void SuffleList(std::list<Tclass> &vec)
{
	int count = vec.size() / 2;

	for (int j = 0; j < count; j++)
	{
		auto p = vec.front();
		vec.pop_front();

		auto it = vec.begin();
		for (int i = 0, n = rand() % vec.size(); i < n; i++)
			it++;
		vec.insert(it, p);
	}

	for (int j = 0; j < count; j++)
	{
		auto p = vec.back();
		vec.pop_back();

		auto it = vec.end();
		for (int i = 0, n = rand() % vec.size(); i < n; i++)
			it--;
		vec.insert(it, p);
	}
}
