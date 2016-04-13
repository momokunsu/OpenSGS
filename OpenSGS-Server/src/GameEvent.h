#pragma once

#include "def.h"
#include "libs/GC.h"

#include <map>
#include <functional>
#include <vector>

//玩家身份
enum class ePlayerStatusType
{
	None = 0,
	Ruler,
	Subject,
	Rebel,
	Spy
};

//流程类型
enum class ePhraseType
{
	None = 0,
	Begin,
	Ready,
	Judge,
	AfterJudge,
	Draw,
	AfterDraw,
	Battle,
	AfterBattle,
	Throw,
	AfterThrow,
	End,
};

//获取卡牌类型
enum class eGetCardType
{
	None = 0,
	Draw,
	FromPlayer,
	FromThrow,
	FromGarbage
};

//玩家事件回调
enum class eGameEvent
{
	None = 0,
	GameStart,
	BattleStart,
	GetPlayerStatus,
	GetCards,
	Phrase,
	EventsPack
};

class GameEvent : public GC
{
	public:
		static GameEvent* create(eGameEvent ev = eGameEvent::None);
		static void initEndian();

		eGameEvent getEvent() { return m_event_id; }
		int getBufferSize() { return m_cur_size; }

		const void* serialize();
		virtual void serializeTo(void* data);
		virtual void unserialize(const void* data);

	protected:
		GameEvent(eGameEvent ev = eGameEvent::None);
		virtual ~GameEvent();

		void writeVal8(uTypeUnion val);
		void writeVal16(uTypeUnion val);
		void writeVal32(uTypeUnion val);
		void writeVal64(uTypeUnion val);

		uTypeUnion readVal8();
		uTypeUnion readVal16();
		uTypeUnion readVal32();
		uTypeUnion readVal64();

		static std::map<eGameEvent, std::function<GameEvent *()>> m_event_creators;

		void reSize();
		void reSize(const void* data);

		char *m_cur_ptr;
		int m_cur_size;
		eGameEvent m_event_id;
};

class EventsPack :public GameEvent
{
	friend class GameEvent;
	public:
		const std::vector<GameEvent*>& getEvents() { return m_events; }
		void addEvent(GameEvent* ev);
		void removeEvent(GameEvent* ev);
		void removeAllEvents();

		void serializeTo(void* data) override;
		void unserialize(const void* data) override;

	private:
		std::vector<GameEvent*> m_events;
		EventsPack() :GameEvent(eGameEvent::EventsPack) {}
};

class EventGameStart :public GameEvent
{
	friend class GameEvent;
	private:
		EventGameStart() :GameEvent(eGameEvent::GameStart) {}
};

class EventBattleStart :public GameEvent
{
	friend class GameEvent;
	private:
		EventBattleStart() :GameEvent(eGameEvent::BattleStart) {}
};

class EventGetPlayerStatus :public GameEvent
{
	friend class GameEvent;
	public:
		std::map<uchar, ePlayerStatusType> statusMap;

		void serializeTo(void* data) override;
		void unserialize(const void* data) override;

	private:
		EventGetPlayerStatus() :GameEvent(eGameEvent::GetPlayerStatus) {}
};

class EventGetCards :public GameEvent
{
	friend class GameEvent;
	public:
		uchar playerID;
		eGetCardType getType;
		std::vector<uint> cards;

		void serializeTo(void* data) override;
		void unserialize(const void* data) override;

	private:
		EventGetCards() :GameEvent(eGameEvent::GetCards) {}
};

class EventPhrase :public GameEvent
{
	friend class GameEvent;
	public:
		uchar playerID;
		ePhraseType type;

		void serializeTo(void* data) override;
		void unserialize(const void* data) override;

	private:
		EventPhrase() :GameEvent(eGameEvent::Phrase) {}
};