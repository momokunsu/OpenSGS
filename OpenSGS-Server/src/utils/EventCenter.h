#pragma once

#include <map>
#include <set>
#include <functional>
#include <memory>

#include "../def.h"
#include "../libs/GC.h"

namespace utli
{
	class EventCenter
	{
		public:
			struct Handle
			{
				int id;
				int cmd;
				int priority;
				std::function<bool(int, void*)> handler;

				Handle();
				Handle(int eid, int p, std::function<bool(int, void*)> &h);

				bool operator < (const Handle & right) const;
			};

			static const Handle& addListener(int cmd, std::function<bool(int, void*)> handler, int priority = 0);
			static bool removeListener(const Handle& handle);

			static void dispatch(int cmd, void* data) { dispatch({ cmd, data }); }
			static void send(int cmd, void* data) { send({ cmd, data }); }
			static void step();

		private:
			struct Event
			{
				int cmd;
				void* data;
			};
			static void dispatch(const EventCenter::Event &event);
			static void send(const EventCenter::Event &event);

			static int m_cur_id;
			static std::map<int, std::set<Handle>> m_listener_pool;
			static std::list<EventCenter::Event> m_event_list;
	};
};

