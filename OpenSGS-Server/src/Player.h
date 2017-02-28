#pragma once

#include <vector>
#include <functional>

#include "GameEvent.h"

struct GameEvent;
enum class ePlayerStatusType;

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

		uchar getLocation() { return m_location; }
		void setLocation(uchar location) { m_location = location; }

		ePlayerStatusType getPlayerStatus() { return m_status; }
		void setPlayerStatus(ePlayerStatusType status) { m_status = status; }

		//int getHandCardsCount() { return (int)m_handcards.size(); }
		//ulong getHandCard(int index) { return m_handcards[index]; }
		std::vector<uint> & getHandCards() { return m_handcards; }

		bool isDead() { return m_is_dead; }

		void dispatchEvent(GameEvent& ev) { eventCallback(ev); }
		virtual void eventCallback(GameEvent& ev) {}

	protected:
		uchar m_id;
		uchar m_location;
		ePlayerStatusType m_status;
		bool m_is_dead;
		uint m_equips[5];
		std::vector<uint> m_handcards;
		std::vector<uint> m_judges;
		std::vector<uint> m_ext_cards;
};
