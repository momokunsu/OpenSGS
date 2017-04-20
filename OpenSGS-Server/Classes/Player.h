#pragma once

#include "libiop/iop_server.h"
#include "SmartPtr.h"
#include "CardVector.h"
#include "CmdBuffer.h"

class GameServer;
class Player : public CardVector
{
	friend class GameServer;

	public:
		Player(io_handle_t h);
		virtual ~Player(void);
		
		inline const char* getAddress() const{return _IPaddress.c_str();}

		inline io_handle_t getNetHandle() const{return _sock;}

		inline ushort getNetPort() const{return _port;}

		inline bool isConnect() const{return _isconnect;}
		inline void setConnectState(bool flag){_isconnect = flag;}

		inline CmdBuffer *getCurCmd() const{return _cmdque.front();};
		inline int getCmdCount() const{return (int)_cmdque.size();};
		CmdBuffer *popCurCmd();
		void pushCmd(CmdBuffer *cmd);

		inline const char *getName() const{return _name.c_str();}
		inline void setName(const char* name){_name = name;}

		inline int getID() const{return _id;}
		//inline void setID(int id){_id = id;}

		inline int getPos() const{return _pos;}
		
		inline uchar getLifePoint() const{return _hp;}
		//inline void setLifePoint(uchar hp){_hp = hp;}
		
		inline uchar getMaxLifePoint() const{return _hpmax;}
		//inline void setLifePoint(uchar hp){_hpmax = hp;}
		
		inline const CardVector* getJudgeCards(int index) const{return _judgecard;}
		//inline void getJudgeCard(Card* card){_judgecard = card;}
		
		void getEquipCard(Card* card[]) const;
		//void setEquipCard(Card* card[]);

	protected:
		//网络相关
		std::queue<CmdBuffer *> _cmdque;
		std::string _IPaddress;
		bool _isconnect;
		io_handle_t _sock;
		ushort _port;

		//游戏相关
		std::string _classname;
		std::string _name;
		uchar _id;
		uchar _hp, _hpmax;
		uchar _pos;
		CardVector *_judgecard;
		Card* _equipcard[5];
};

