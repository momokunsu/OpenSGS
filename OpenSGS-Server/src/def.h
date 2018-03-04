 #pragma once

#include <vector>
#include <list>
#include <algorithm>
#include <stdlib.h>
#include <memory>
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
	uint intVal[2];
	float floatVal[2];
	void* ptrVal[2];
	uint64 int64Val;
	double doubleVal;
} uTypeUnion;

#pragma pack(pop)

//智能指针
#define UPtr(t) std::unique_ptr<t>
#define UNew(t) std::unique_ptr<t>(new t)
#define UNewArr(t, c) std::unique_ptr<t[]>(new t[c])
#define SPtr(t) std::shared_ptr<t>
#define SNew(t) std::shared_ptr<t>(new t)
#define SNewArr(t, c) std::shared_ptr<t[]>(new t[c])
#define MvPtr(p) std::move(p)

// 属性访问器
#define Getter(n, m, t) inline t get##n() { return m; } 
#define Setter(n, m, t) inline void set##n(t p) { m = p; } 
#define Visitor(n, m, t) private: t m;\
												 public: Getter(n, m, t) Setter(n, m, t)

template<class Tclass>
void SuffleVector(std::vector<Tclass> &vec)
{
	std::random_shuffle(vec.begin(), vec.end());
	std::random_shuffle(vec.begin(), vec.end());
}

template<class Tclass>
void SuffleList(std::list<Tclass> &vec)
{
	int count = (int)vec.size() / 2;

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
