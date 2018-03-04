#include "EventCenter.h"

using namespace utli;


EventCenter::Handle::Handle()
{
	id = m_cur_id++;
	std::unique_ptr<int> a, b;
	b = std::move(a);
}

EventCenter::Handle::Handle(int eid, int p, std::function<bool(int, void*)>& h)
{
	id = m_cur_id++;
	cmd = eid;
	priority = p;
	handler = h;
}

bool EventCenter::Handle::operator < (const Handle & right) const
{
	return right.priority == priority ? right.id - id < 0 : right.priority - priority < 0;
}


int EventCenter::m_cur_id = 1;
std::map<int, std::set<EventCenter::Handle>> EventCenter::m_listener_pool;

const EventCenter::Handle& EventCenter::addListener(int cmd, std::function<bool(int, void*)> handler, int priority)
{
	auto& handler_list = m_listener_pool[cmd];
	return *(handler_list.insert({ cmd, priority, handler }).first);
}

bool EventCenter::removeListener(const Handle & handle)
{
	auto& handler_list = m_listener_pool[handle.cmd];
	return handler_list.erase(handle) > 0;
}

void EventCenter::dispatch(const EventCenter::Event &event)
{
	auto listeners = m_listener_pool[event.cmd];
	for (auto& i : listeners)
	{
		i.handler(event.cmd, event.data);
	}
}

void utli::EventCenter::send(const EventCenter::Event & event)
{
	m_event_list.push_back(event);
}

void EventCenter::step()
{
	auto& event = m_event_list.front();
	auto& listeners = m_listener_pool[event.cmd];
	for (auto& l : listeners)
	{
		l.handler(event);
	}
}
