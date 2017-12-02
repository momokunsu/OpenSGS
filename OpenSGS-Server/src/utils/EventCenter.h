#pragma once

#include <map>
#include <set>
#include <functional>
#include <memory>

//#include ""

namespace utli
{
	class EventCenter
	{
		public:
			struct Event
			{
				int cmd;
			};
			struct Handle
			{
				int id;
				int cmd;
				int priority;
				std::function<bool(std::unique_ptr<EventCenter::Event>)> handler;

				Handle();
				Handle(int eid, int p, std::function<bool(std::unique_ptr<EventCenter::Event>)> &h);

				bool operator < (const Handle & right) const;
			};

			static const Handle& addListener(int cmd, std::function<bool(std::unique_ptr<EventCenter::Event>)> handler, int priority = 0);
			static bool removeListener(static const Handle& handle);

		private:
			static int m_cur_id;
			static std::map<int, std::set<Handle>> m_event_pool;
	};
};

