#pragma once

#include "GameServer.h"


class GameClient : public SmartPtr
{
	public:
		GameClient(void);
		virtual ~GameClient(void);

		bool setSeverAddress(const char *address);
		bool setSeverPort(ushort port);
		bool connectToSever(int waittime);

		inline void setCmdComed(CmdComed func){_ccomcb = func;}
		void sendCmd(CmdBuffer *buffer);

	private:
		std::string _severip;
		ushort _severport;
		bool _isconnect;

		CmdComed _ccomcb;
};
