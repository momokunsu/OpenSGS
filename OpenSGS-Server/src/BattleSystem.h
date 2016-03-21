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

		bool initGame();

		void startGame();
		void distributeStatus();
		void startBattle();
		void phraseStep();

		void skipThisTurn();
		
	private:
		void handlePhrase(Player *player, ePhraseType ptype);

		std::vector<ePlayerStatusType> m_statusgroup;
		std::vector<Player*> m_players;

		int m_cur_player;
		ePhraseType m_cur_phrase;

		std::vector<uint> m_card_deck;
		std::vector<uint> m_card_recycle_bin;

		int m_global_drawcount;
		int m_drawcount;
};

