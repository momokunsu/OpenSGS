#pragma once
#include "Player.h"
#include "Cards.h"

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

struct UseCardData
{
	Player* player;
	Card* card;

	UseCardData()
	{
		memset(this, 0, sizeof(UseCardData));
	}
};

class BattleSystem
{
	public:
		BattleSystem();
		virtual ~BattleSystem();

		bool addPlayer(Player* player);

		bool setPlayerLocation(Player* player, int pos);
		void sufflePlayersLocation();

		std::list<uint>& cardsDeck() { return m_card_deck; }
		void shuffleCardsDeck();

		bool initGame();

		void startGame();
		void dealStatus();
		void dealCards();
		void startBattle();
		void phraseStep();
		void handleUseCardStack();

		void drawCards(uchar playerid, int count, int index = 0);
		void useCard(uchar userid, uchar objectid, int cardpos);
		void skipThisTurn();
		
	private:
		void broadcastEvent(GameEvent& ev);
		void handlePhrase(Player *player, ePhraseType ptype);
		uint drawCard();

		std::vector<ePlayerStatusType> m_statusgroup;
		std::vector<Player*> m_players;
		std::map<uchar, Player*> m_id_players;

		int m_cur_player;
		ePhraseType m_cur_phrase;

		std::list<uint> m_card_deck;
		std::list<uint> m_card_recycle_bin;
		std::vector<UseCardData> m_use_card_stack;

		int m_global_drawcount;
		int m_drawcount;
};

