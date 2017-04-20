#pragma once

#include "SmartPtr.h"

//Ӧ��ָ��
//����ָ��
#define CMD_PLAYERINIT			0x0001 //�����Ϣ��ʼ��
#define CMD_GAMESTART		0x0002 //��Ϸ��ʼ
#define CMD_GAMEFINISH		0x0003 //��Ϸ����
#define CMD_GAMETIME			0x0004 //��Ϸ�׶�
#define CMD_DRAWCARD		0x0005 //����
#define CMD_USECARD			0x0006 //����
#define CMD_THROWCARD		0x0007 //����
#define CMD_SELECTCARD		0x0008 //ѡ��
#define CMD_GAINCARD			0x0009 //����

class CmdBuffer : public SmartPtr
{
	friend class CmdBuffer;
	public:
		CmdBuffer(void);
		virtual ~CmdBuffer(void);
		
		void init();

		inline const void* getBufPtr() const{return _buffer;}
		int getBufSize() const;

		int getPos() const;
		void setPos(int pos);

		ushort getCmdCode() const;
		void setCmdCode(ushort cmd);

		int getDataLen() const;
		void setDataLenForPos();

		void resetPos();

		bool canHandle() const;
		void copyBuffer(CmdBuffer *buffer);
		int recvData(void *data, int len);

		uchar readByte();
		ushort readWord();
		ulong readDword();
		uint64 readDDword();
		float readFloat();
		double readDouble();
		const char *readText();
		void readData(void *data, int len);

		void writeByte(uchar c);
		void writeWord(ushort s);
		void writeDword(ulong l);
		void writeDDword(uint64 ll);
		void writeFloat(float f);
		void writeDouble(double d);
		int writeText(const char *str);
		void writeData(void *data, int len);

	private:
		void *_pos, *_wpos;
		char _buffer[64];
};

