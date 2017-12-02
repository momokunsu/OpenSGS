#pragma once
#include "Player.h"
#include "Cards.h"


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
		static BattleSystem* getInstance();

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

		void drawCards(uchar playerId, int count, int index = 0);
		void useCard(uchar userId, uint cardPos, uchar targetId = 0);
		void useCard(uchar userId, Card* card, uchar targetId = 0, bool canThrow = true);
		void throwCard(const std::vector<uint>& cards, eThrowCardType throwType = eThrowCardType::Used);
		void handleUseCards();
  
		void skipThisTurn();
		
	private:
		BattleSystem();
		virtual ~BattleSystem();

		void broadcastEvent(GameEvent& ev);
		void handlePhrase(Player *player, ePhraseType ptype);
		uint drawCard();

		static BattleSystem *self;

		std::vector<ePlayerStatusType> m_statusgroup;
		std::vector<Player*> m_players;
		std::map<uchar, Player*> m_id_players;

		int m_cur_player;
		ePhraseType m_cur_phrase;

		std::list<uint> m_card_deck;
		std::list<uint> m_card_recycle_bin;
  
		std::list<EventUseCard> m_used_card_list;

		int m_global_drawcount;
		int m_drawcount;
};

