#pragma once
#include <vector>
#include "Player.h"


class BattleSystem
{
	public:
		BattleSystem();
		virtual ~BattleSystem();

		bool addPlayer(Player* player);
		bool setPlayerLocal(Player* player, int pos);
		void sufflePlayersLocal();

		bool startGame();
		void distributeStatus();
		void startBattle();
		void phraseStep();

		void skipThisRound();
		
	private:
		void handlePhrase(Player *player, ePhraseType ptype);

		std::vector<ePlayerStatusType> m_statusgroup;

		int m_cur_player;
		std::vector<Player*> m_players;
		std::vector<uchar> m_players_local;

		std::vector<ulong> m_cardgroup;
		std::vector<ulong> m_card_recycle_bin;

		ePhraseType m_cur_phrase;
};

