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

//����ת��������
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


//ʱ�����
#define TIME_OWN		0x40000000	//�Լ���ʱ��
#define TIME_OTHER	0x80000000	//������ҵ�ʱ��

#define TIME_START		0x00000001	//��ʼ�׶�
#define TIME_READY	0x00000002	//׼���׶�
#define TIME_JUDGE	0x00000004	//�ж��׶�
#define TIME_DRAW		0x00000008 	//���ƽ׶�
#define TIME_USE			0x00000010	//���ƽ׶�
#define TIME_THROW	0x00000020	//���ƽ׶�
#define TIME_END		0x00000040	//�����׶�

#define TIME_GAME_START		0x00000080	//��Ϸ��ʼʱ
#define TIME_GAME_END			0x00000100	//��Ϸ����ʱ

#define TIME_USETARGET						0x00000200	//��Ϊ�Ƶĵ�������ʱ
#define TIME_SIKLLTARGET					0x00000400	//��Ϊ���ܵĵ�������ʱ
#define TIME_COMPARETARGET			0x00004000	//��Ϊƴ��ĵ�������ʱ
#define TIME_MULUSETARGET				0x00000800	//��Ϊ�Ƶ�����һ������ʱ
#define TIME_MULSIKLLTARGET			0x00001000	//��Ϊ���ܵ�����һ������ʱ
#define TIME_MULCOMPARETARGET	0x00008000	//��Ϊƴ�������һ������ʱ

#define TIME_TURNFRONT	0x00010000		//����Ϊ����ʱ
#define TIME_TURNBACK		0x00020000		//����Ϊ����ʱ

#define TIME_HURTOWN			0x00040000		//����˺�ʱ
#define TIME_EROSIONOWN	0x00080000		//���������ʧʱ
#define TIME_RECOVER			0x00100000		//�ָ�����ʱ
#define TIME_ADDMAXHP		0x00200000		//������������ʱ
#define TIME_SUBMAXHP			0x00400000		//�۳���������ʱ
#define TIME_DIYINGOWN		0x00800000		//Ƶ��ʱ
#define TIME_DEADOWN			0x01000000		//����ʱ

//��Ƭ����
enum eCardType
{
	General = 0, 
	Base = 1,
	Scheme = 2,
	Equip = 3,
	Treasure = 4
};

//�����������ֽ�˳��ת��
extern std::function<void (TYPEUNION &u, int size)> _ntoh;
extern std::function<void (TYPEUNION &u, int size)> _hton;

void initEndianChage();