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

	private:
		std::vector<ePlayerStatusType> m_statusgroup;

		int m_cur_player;
		std::vector<Player*> m_players;
		std::vector<unsigned char> m_players_local;

		std::vector<unsigned long> m_cardgroup;
		std::vector<unsigned long> m_card_recycle_bin;
};

