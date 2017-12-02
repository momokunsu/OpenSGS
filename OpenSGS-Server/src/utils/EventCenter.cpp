#include "EventCenter.h"

using namespace utli;


EventCenter::Handle::Handle()
{
	id = m_cur_id++;
	std::unique_ptr<int> a, b;
	b = std::move(a);
}

EventCenter::Handle::Handle(int eid, int p, std::function<bool(std::unique_ptr<EventCenter::Event>)>& h)
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
std::map<int, std::set<EventCenter::Handle>> EventCenter::m_event_pool;

const EventCenter::Handle& EventCenter::addListener(int cmd, std::function<bool(std::unique_ptr<EventCenter::Event>)> handler, int priority)
{
	auto& handler_list = m_event_pool[cmd];
	return *(handler_list.insert({ cmd, priority, handler }).first);
}

bool EventCenter::removeListener(const Handle & handle)
{
	auto& handler_list = m_event_pool[handle.cmd];
	return handler_list.erase(handle) > 0;
}
