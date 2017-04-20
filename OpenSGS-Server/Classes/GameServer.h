#pragma once

#include "SmartPtr.h"
#include "Player.h"

typedef std::function<void (Player *, uchar, bool)> PlayerConnected;
typedef std::function<void (Player *, CmdBuffer *)> CmdComed;
typedef std::function<Player *()> PlayerConstructor;

class GameServer : public SmartPtr
{
	public:
		GameServer(void);
		virtual ~GameServer(void);

		inline void setCmdComed(CmdComed func){_ccomcb = func;}

		void addLocalPlayer(Player *player);
		void removePlayer(Player *player);

		inline int getPlayerCount() const{return _playersmap.size();}
		inline Player *getPlayerFromID(int id){return _playersmap[id];}
		void getAllPlayers(std::vector<Player*> &vec);

		static void getLocalAddress(std::vector<std::string> &ippool);

		bool setHostAdress(const char *ip);
		bool setHostPort(int port);

		void listen(PlayerConnected cntevent = nullptr, PlayerConstructor constor = nullptr);
		void endListen();
		inline bool isListen() const{return _islisten;}

		void sendCmd(Player *player, CmdBuffer *buffer);
		void broadcastCmd(CmdBuffer *buffer);

	private:
		inline void _setID(uchar id){_playersmap[id]->_id = id;}

		friend int dataRecv(iop_base_t *base,int id,char *data,int len,void *arg);
		friend void onConnect(iop_base_t *base, int id, void *arg);
		friend void onDestroy(iop_base_t *base, int id, void *arg);
		friend int onError(iop_base_t *base, int id,int err, void *arg);

		iop_base_t *_iopbase;
		int _serverid;
		PlayerConstructor _playercstor;
		PlayerConnected _cntcb;
		CmdComed _ccomcb;

		std::string _hostaddress;
		int _hostport;
		bool _islisten;
		std::map<uchar , Player*> _playersmap;
};