#pragma once

#include <vector>

#include "def.h"
#include "libs/GlobalBuffer.h"
#include <map>

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
	Phrase
};

class GameEvent
{
	public:
		GameEvent(eGameEvent ev);
		virtual ~GameEvent();

		void setEvent(eGameEvent ev){ m_event_id = ev; }
		eGameEvent getEvent() { return m_event_id; }

		const void* serialize();
		virtual void serialize(void* data);
		virtual void unserialize(const void* data);

		static void initEndian();
		static int getBufferSize(const void* data);

	protected:
		void writeVal8(uTypeUnion val);
		void writeVal16(uTypeUnion val);
		void writeVal32(uTypeUnion val);
		void writeVal64(uTypeUnion val);

		uTypeUnion readVal8();
		uTypeUnion readVal16();
		uTypeUnion readVal32();
		uTypeUnion readVal64();

		void reSize();
		void reSize(const void* data);

		char *m_cur_ptr;
		int m_cur_size;
		eGameEvent m_event_id;
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

		void serialize(void* data) override;
		void unserialize(const void* data) override;
};

class EventGetCards :public GameEvent
{
	public:
		EventGetCards() :GameEvent(eGameEvent::GetCards) {}

		uchar playerID;
		std::vector<uint> cards;

		void serialize(void* data) override;
		void unserialize(const void* data) override;
};

class EventPhrase :public GameEvent
{
	public:
		EventPhrase() :GameEvent(eGameEvent::Phrase) {}

		uchar playerID;
		ePhraseType type;

		void serialize(void* data) override;
		void unserialize(const void* data) override;
};