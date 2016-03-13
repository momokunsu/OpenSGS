#pragma once

#include <vector>
#include <functional>

//玩家身份
enum class ePlayerStatusType
{
	None = 0,
	Ruler,
	Subject,
	Rebel,
	Spy
};

//玩家装备牌索引
enum class eEquipsIndex
{
	Wepon = 0,
	Armor,
	ActHorse,
	DefHorse,
	Treasure
};

//玩家事件回调
enum class ePlayerEvent
{
	None = 0,
	BattleStart,
	GetPalyerStatus,
	GetCards
};

class Player
{
	public:
		Player();
		virtual ~Player();

		unsigned char getID() { return m_id; }
		unsigned char setID(unsigned char id) { m_id = id; }

		int getHandCardsCount() { return m_handcards.size(); }
		unsigned long getHandCard(int index) { return m_handcards[index]; }
		void suffle();

		void setEventCallback(std::function<void(ePlayerEvent, void *)> event) { mc_event_callback = event; }
		void dispatchEvent(ePlayerEvent etype, void *data) { mc_event_callback(etype, data); }

	private:
		unsigned char m_id;
		ePlayerStatusType *m_status;
		unsigned long m_equips[5];
		std::vector<unsigned long> m_handcards;
		std::vector<unsigned long> m_judges;
		std::vector<unsigned long> m_ext_cards;
		std::function<void(ePlayerEvent, void *)> mc_event_callback;
};
