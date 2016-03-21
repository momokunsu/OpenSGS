#include "GameEvent.h"
#include <functional>

void endianChage(uTypeUnion &u, int size)
{
	int dsize = size / 2;
	for(int i = 0; i < dsize; i++)
	{
		uchar t = u.charVal[i];
		u.charVal[i] = u.charVal[size-i-1];
		u.charVal[size-i-1] = t;
	}
}

void endianNotChage(uTypeUnion &u, int size)
{
}

std::function<void(uTypeUnion &u, int size)> _endianhandler = endianNotChage;

GameEvent::GameEvent(eGameEvent ev)
{
	m_event_id = ev;
}

GameEvent::~GameEvent()
{
}

void GameEvent::initEndian()
{
	uTypeUnion u;
	u.intVal[0] = 0xFF0000FF;
	if(u.shortVal[0] == 0x00FF && u.shortVal[1] == 0xFF00)
		_endianhandler = endianNotChage;
	else
		_endianhandler = endianChage;
}

int GameEvent::getBufferSize(const void * data)
{
	return *((int*)GlobalBuffer);
}

void GameEvent::writeVal8(uTypeUnion val)
{
	*((uchar*)m_cur_ptr) = val.charVal[0];
	m_cur_size++;
	m_cur_ptr++;
}

void GameEvent::writeVal16(uTypeUnion val)
{
	_endianhandler(val, 2);
	*((ushort*)m_cur_ptr) = val.shortVal[0];
	m_cur_size += 2;
	m_cur_ptr += 2;
}

void GameEvent::writeVal32(uTypeUnion val)
{
	_endianhandler(val, 4);
	*((uint*)m_cur_ptr) = val.intVal[0];
	m_cur_size += 4;
	m_cur_ptr += 4;
}

void GameEvent::writeVal64(uTypeUnion val)
{
	_endianhandler(val, 8);
	*((uint64*)m_cur_ptr) = val.int64Val;
	m_cur_size += 8;
	m_cur_ptr += 8;
}

uTypeUnion GameEvent::readVal8()
{
	uTypeUnion val;
	val.charVal[0] = *((uchar*)m_cur_ptr);
	m_cur_size ++;
	m_cur_ptr ++;
	return val;
}

uTypeUnion GameEvent::readVal16()
{
	uTypeUnion val;
	val.shortVal[0] = *((ushort*)m_cur_ptr);
	_endianhandler(val, 2);
	m_cur_size += 2;
	m_cur_ptr += 2;
	return val;
}

uTypeUnion GameEvent::readVal32()
{
	uTypeUnion val;
	val.intVal[0] = *((uint*)m_cur_ptr);
	_endianhandler(val, 4);
	m_cur_size += 4;
	m_cur_ptr += 4;
	return val;
}

uTypeUnion GameEvent::readVal64()
{
	uTypeUnion val;
	val.int64Val = *((uint64*)m_cur_ptr);
	_endianhandler(val, 8);
	m_cur_size += 8;
	m_cur_ptr += 8;
	return val;
}

void GameEvent::reSize()
{
	*((int*)GlobalBuffer) = m_cur_size;
}


void * GameEvent::serialize()
{
	m_cur_size = 0;
	m_cur_ptr = GlobalBuffer;

	uTypeUnion val;

	val.intVal[0] = 0;
	writeVal32(val);

	val.intVal[0] = (ulong)m_event_id;
	writeVal32(val);

	reSize();
	return GlobalBuffer;
}

void GameEvent::unserialize(const void * data)
{
	m_cur_size = 0;
	m_cur_ptr = GlobalBuffer;

	memcpy(GlobalBuffer, data, getBufferSize(data));

	uTypeUnion val;

	readVal32();//size

	val = readVal32();
	m_event_id = (eGameEvent)val.intVal[0];
}

void * EventGetPlayerStatus::serialize()
{
	GameEvent::serialize();

	uTypeUnion val;

	val.shortVal[0] = statusMap.size();
	writeVal16(val);
	for (auto it : statusMap)
	{
		val.charVal[0] = it.first;
		writeVal8(val);
		val.charVal[0] = (uchar)it.second;
		writeVal8(val);
	}

	reSize();
	return GlobalBuffer;
}

void EventGetPlayerStatus::unserialize(const void * data)
{
	GameEvent::unserialize(data);

	uTypeUnion val;

	val = readVal16();
	for (int i = 0, size = val.shortVal[0]; i < size; i++)
	{
		val = readVal8();
		auto player = val.charVal[0];
		val = readVal8();
		auto status = val.charVal[0];
		statusMap[player] = (ePlayerStatusType)status;
	}
}

void * EventGetCards::serialize()
{
	GameEvent::serialize();

	uTypeUnion val;

	val.charVal[0] = playerID;
	writeVal8(val);

	val.shortVal[0] = cards.size();
	writeVal16(val);
	for (auto it : cards)
	{
		val.intVal[0] = it;
		writeVal32(val);
	}

	reSize();
	return GlobalBuffer;
}

void EventGetCards::unserialize(const void * data)
{
	GameEvent::unserialize(data);

	uTypeUnion val;

	val = readVal8();
	playerID = val.charVal[0];

	val = readVal16();
	cards.clear();
	for (int i = 0, size = val.shortVal[0]; i < size; i++)
	{
		val = readVal32();
		cards.push_back(val.intVal[0]);
	}
}

void * EventPhrase::serialize()
{
	GameEvent::serialize();

	uTypeUnion val;

	val.charVal[0] = playerID;
	writeVal8(val);

	val.charVal[0] = (uchar)type;
	writeVal8(val);

	reSize();
	return GlobalBuffer;
}

void EventPhrase::unserialize(const void * data)
{
	GameEvent::unserialize(data);

	uTypeUnion val;

	val = readVal8();
	playerID = val.charVal[0];

	val = readVal8();
	type = (ePhraseType)val.charVal[0];
}
