#pragma once

#include <list>
#include "GameEvent.h"

class EventStack
{
	public:
		EventStack();
		~EventStack();

		void pushEvent(GameEvent *event);
		void handleEvent();

	private:
		std::list<GameEvent *> m_stack;
};

