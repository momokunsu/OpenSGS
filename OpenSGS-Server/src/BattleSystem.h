#pragma once
#include "Player.h"


struct BattleInfo
{

};

class BattleSystem
{
	public:
		BattleSystem();
		virtual ~BattleSystem();

		bool addPlayer(Player* player);
		bool setPlayerLocal(Player* player, int pos);
		void sufflePlayersLocal();

		bool initBattleInfo();

		void startGame();
		void distributeStatus();
		void startBattle();
		void phraseStep();

		void skipThisRound();
		
	private:
		void handlePhrase(Player *player, ePhraseType ptype);

		std::vector<ePlayerStatusType> m_statusgroup;
		std::vector<Player*> m_players;

		int m_cur_player;
		ePhraseType m_cur_phrase;

		std::vector<ulong> m_cardgroup;
		std::vector<ulong> m_card_recycle_bin;

		int m_global_drawcount;
		int m_drawcount;
};

