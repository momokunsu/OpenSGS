#include "EventStack.h"



EventStack::EventStack()
{
}


EventStack::~EventStack()
{
}

void EventStack::pushEvent(GameEvent * event)
{
	m_stack.push_back(event);
}

void EventStack::handleEvent()
{
	auto event = m_stack.back();
}
