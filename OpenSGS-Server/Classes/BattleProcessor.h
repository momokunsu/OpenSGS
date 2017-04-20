#pragma once

#include "Player.h"
#include "CmdBuffer.h"


class BattleProcessor : public SmartPtr
{
	public:
		BattleProcessor(void);
		virtual ~BattleProcessor(void) = 0;
		
		virtual bool start() = 0;
		virtual void Process() = 0;

		virtual bool addPlayer(Player *player, int pos = -1);
		virtual void setCardgroup(CardVector *group);
		CardVector *getCardgroup() const{return _cardgroup;}

		virtual CmdBuffer *getResult();

		inline uint getMaxPlayers() const{return _players.size();}
		inline void setMaxPlayers(uint n){_players.resize(n);}

	protected:
		bool _isstart;

		CardVector *_cardgroup;
		std::queue<CmdBuffer *> _cmdque;
		std::stack<CmdBuffer *> _handlestack;
		std::vector<Player *> _players;
		std::list<Player *> _lifeplayers;
		std::list<Player *>::iterator _curplayer;
		uchar _curtime;
		uint _palyercount;
};

