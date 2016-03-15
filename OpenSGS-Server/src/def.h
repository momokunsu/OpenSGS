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
	uint64 int64Val;
	double doubleVal;
}TYPEUNION;

#pragma pack(pop)


//时刻旗标
#define TIME_OWN		0x40000000	//自己的时刻
#define TIME_OTHER	0x80000000	//其他玩家的时刻

#define TIME_START		0x00000001	//开始阶段
#define TIME_READY	0x00000002	//准备阶段
#define TIME_JUDGE	0x00000004	//判定阶段
#define TIME_DRAW		0x00000008 	//抽牌阶段
#define TIME_USE			0x00000010	//出牌阶段
#define TIME_THROW	0x00000020	//弃牌阶段
#define TIME_END		0x00000040	//结束阶段

#define TIME_GAME_START		0x00000080	//游戏开始时
#define TIME_GAME_END			0x00000100	//游戏结束时

#define TIME_USETARGET						0x00000200	//成为牌的单个对象时
#define TIME_SIKLLTARGET					0x00000400	//成为技能的单个对象时
#define TIME_COMPARETARGET			0x00004000	//成为拼点的单个对象时
#define TIME_MULUSETARGET				0x00000800	//成为牌的其中一个对象时
#define TIME_MULSIKLLTARGET			0x00001000	//成为技能的其中一个对象时
#define TIME_MULCOMPARETARGET	0x00008000	//成为拼点的其中一个对象时

#define TIME_TURNFRONT	0x00010000		//被翻为正面时
#define TIME_TURNBACK		0x00020000		//被翻为背面时

#define TIME_HURTOWN			0x00040000		//造成伤害时
#define TIME_EROSIONOWN	0x00080000		//造成体力流失时
#define TIME_RECOVER			0x00100000		//恢复体力时
#define TIME_ADDMAXHP		0x00200000		//增加体力上限时
#define TIME_SUBMAXHP			0x00400000		//扣除体力上限时
#define TIME_DIYINGOWN		0x00800000		//频死时
#define TIME_DEADOWN			0x01000000		//死亡时

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
extern std::function<void (TYPEUNION &u, int size)> _ntoh;
extern std::function<void (TYPEUNION &u, int size)> _hton;

void initEndianChage();