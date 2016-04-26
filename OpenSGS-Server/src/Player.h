#pragma once

#include <vector>
#include <functional>

#include "GameEvent.h"

//玩家装备牌索引
enum class eEquipsIndex
{
	Wepon = 0,
	Armor,
	ActHorse,
	DefHorse,
	Treasure
};

class Player : public GC
{
	public:
		Player();
		virtual ~Player();

		uchar getID() { return m_id; }
		void setID(uchar id) { m_id = id; }

		int getHandCardsCount() { return (int)m_handcards.size(); }
		ulong getHandCard(int index) { return m_handcards[index]; }
		void suffle();

		bool isDead() { return m_is_dead; }

		void dispatchEvent(GameEvent *etype) { EventCallback(etype); }
		virtual void EventCallback(GameEvent* ev) {}

	protected:
		uchar m_id;
		ePlayerStatusType m_status;
		bool m_is_dead;
		ulong m_equips[5];
		std::vector<ulong> m_handcards;
		std::vector<ulong> m_judges;
		std::vector<ulong> m_ext_cards;
};
