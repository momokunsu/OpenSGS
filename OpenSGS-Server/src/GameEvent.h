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

class GameEvent
{
	public:
		GameEvent(eGameEvent ev = eGameEvent::None);
		virtual ~GameEvent();

		eGameEvent getEvent() { return m_event_id; }
		int getBufferSize() { return m_cur_size; }

		const void* serialize();
		virtual void serializeTo(void* data);
		virtual void unserialize(const void* data);

		static void initEndian();
		static GameEvent* create(eGameEvent ev);

	protected:
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
	public:
		EventsPack() :GameEvent(eGameEvent::EventsPack) {}

		std::vector<GameEvent*> events;

		void serializeTo(void* data) override;
		void unserialize(const void* data) override;
};

class EventGameStart :public GameEvent
{
	public:
		EventGameStart() :GameEvent(eGameEvent::GameStart) {}
};

class EventBattleStart :public GameEvent
{
	public:
		EventBattleStart() :GameEvent(eGameEvent::BattleStart) {}
};

class EventGetPlayerStatus :public GameEvent
{
	public:
		EventGetPlayerStatus() :GameEvent(eGameEvent::GetPlayerStatus) {}

		std::map<uchar, ePlayerStatusType> statusMap;

		void serializeTo(void* data) override;
		void unserialize(const void* data) override;
};

class EventGetCards :public GameEvent
{
	public:
		EventGetCards() :GameEvent(eGameEvent::GetCards) {}

		uchar playerID;
		std::vector<uint> cards;

		void serializeTo(void* data) override;
		void unserialize(const void* data) override;
};

class EventPhrase :public GameEvent
{
	public:
		EventPhrase() :GameEvent(eGameEvent::Phrase) {}

		uchar playerID;
		ePhraseType type;

		void serializeTo(void* data) override;
		void unserialize(const void* data) override;
};