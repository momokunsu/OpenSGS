#pragma once
#include "Player.h"


struct BattleData
{
	std::vector<uint> deck;         //牌组
	std::vector<uint> heroDeck;  //武将牌组
	std::vector<uint> recycleBin; //弃牌区

	int globalDrawCount;  //全局抽卡数
	int drawCount;	          //本回合抽卡数

	BattleData()
	{
		memset(this, 0, sizeof(BattleData));
	}
};

class BattleSystem
{
	public:
		BattleSystem();
		virtual ~BattleSystem();

		bool addPlayer(Player* player);
		bool setPlayerLocal(Player* player, int pos);
		void sufflePlayersLocal();

		void setDeck(std::vector<uint> deck);

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

