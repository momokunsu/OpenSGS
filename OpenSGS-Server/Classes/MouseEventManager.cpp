#include "MouseEventManager.h"

using namespace cocos2d;

MouseEventManager *MouseEventManager::self = nullptr;


MouseEventManager::MouseEventManager(void)
{
	_dispatcher = Director::getInstance()->getEventDispatcher();
	auto elm = EventListenerMouse::create();
	_movehandle = _uphandle = _downhandle = _scrollhandle = false;
	_whoenter = nullptr;
	elm->onMouseMove = [=](Event *e)
	{
		_movehandle = false;
	};
	elm->onMouseUp = [=](Event *e)
	{
		_uphandle = false;
	};
	elm->onMouseDown = [=](Event *e)
	{
		_downhandle = false;
	};
	elm->onMouseScroll = [=](Event *e)
	{
		_scrollhandle = false;
	};
	_dispatcher->addEventListenerWithFixedPriority(elm, -10000);
}

MouseEventManager::~MouseEventManager(void)
{
	self = nullptr;
}

MouseEventManager *MouseEventManager::getInstance()
{
	if(!self)
	{
		self = new MouseEventManager();
		self->retain();
	}
	return self;
}

void MouseEventManager::addMouseEnterEvent(Node *node, MouseEventCallBack evmouse)
{
	auto elm = EventListenerMouse::create();
	elm->onMouseMove = [=](Event *e)
	{
		auto em = (EventMouse *)e;
		if(!_movehandle)
		{
			if(em->getCurrentTarget() == node)
			{
				auto locationInNode = node->convertToNodeSpace(Vec2(em->getCursorX(), em->getCursorY()));
				auto s = node->getContentSize();
				Rect hitrect = Rect(0, 0, s.width, s.height);
				if(hitrect.containsPoint(locationInNode) && !_whoenter)
				{
					_whoenter = node;
					_movehandle = evmouse(em);
				}
			}
		}
	};
	_dispatcher->addEventListenerWithSceneGraphPriority(elm, node);
}

void MouseEventManager::addMouseLeaveEvent(Node *node, MouseEventCallBack evmouse)
{
	auto elm = EventListenerMouse::create();
	elm->onMouseMove = [=](Event *e)
	{
		auto em = (EventMouse *)e;
		if(!_movehandle)
		{
			if(em->getCurrentTarget() == node)
			{
				auto locationInNode = node->convertToNodeSpace(Vec2(em->getCursorX(), em->getCursorY()));
				auto s = node->getContentSize();
				Rect hitrect = Rect(0, 0, s.width, s.height);
				if(!hitrect.containsPoint(locationInNode) && _whoenter == node)
				{
					_whoenter = nullptr;
					_movehandle = evmouse(em);
				}
			}
		}
	};
	_dispatcher->addEventListenerWithSceneGraphPriority(elm, node);
}

void MouseEventManager::addMouseMoveEvent(Node *node, MouseEventCallBack evmouse)
{
	auto elm = EventListenerMouse::create();
	elm->onMouseMove = [=](Event *e)
	{
		auto em = (EventMouse *)e;
		if(!_movehandle)
		{
			if(em->getCurrentTarget() == node)
			{
				auto locationInNode = node->convertToNodeSpace(Vec2(em->getCursorX(), em->getCursorY()));
				auto s = node->getContentSize();
				Rect hitrect = Rect(0, 0, s.width, s.height);
				if(!hitrect.containsPoint(locationInNode))
				{
					_movehandle = evmouse(em);
				}
			}
		}
	};
	_dispatcher->addEventListenerWithSceneGraphPriority(elm, node);
}

void MouseEventManager::addMouseUpEvent(Node *node, MouseEventCallBack evmouse)
{
	auto elm = EventListenerMouse::create();
	elm->onMouseUp = [=](Event *e)
	{
		auto em = (EventMouse *)e;
		if(!_uphandle)
		{
			if(em->getCurrentTarget() == node)
			{
				auto locationInNode = node->convertToNodeSpace(Vec2(em->getCursorX(), em->getCursorY()));
				auto s = node->getContentSize();
				Rect hitrect = Rect(0, 0, s.width, s.height);
				if(hitrect.containsPoint(locationInNode))
				{
					_uphandle = evmouse(em);
				}
			}
		}
	};
	_dispatcher->addEventListenerWithSceneGraphPriority(elm, node);
}
void MouseEventManager::addMouseDownEvent(cocos2d::Node *node, MouseEventCallBack evmouse)
{
	auto elm = EventListenerMouse::create();
	elm->onMouseDown = [=](Event *e)
	{
		auto em = (EventMouse *)e;
		if(!_downhandle)
		{
			if(em->getCurrentTarget() == node)
			{
				auto locationInNode = node->convertToNodeSpace(Vec2(em->getCursorX(), em->getCursorY()));
				auto s = node->getContentSize();
				Rect hitrect = Rect(0, 0, s.width, s.height);
				if(hitrect.containsPoint(locationInNode))
				{
					_downhandle = evmouse(em);
				}
			}
		}
	};
	_dispatcher->addEventListenerWithSceneGraphPriority(elm, node);
}
void MouseEventManager::addMouseDoubleClickEvent(Node *node, MouseEventCallBack evmouse)
{
	auto elm = EventListenerMouse::create();
	elm->onMouseUp = [=](Event *e)
	{
		auto em = (EventMouse *)e;
		if(!_uphandle)
		{
			static auto prepos = Vec2(em->getCursorX(), em->getCursorY());
			static auto pretime = clock();
			auto curpos = Vec2(em->getCursorX(), em->getCursorY());
			auto curtime = clock();
			auto deltapos = curpos - prepos;
			auto deltatime = curtime - pretime;
			if(abs(deltapos.x) < _dclickrange.width && abs(deltapos.y) < _dclickrange.height && deltatime < _dclickdelay)
			{
				if(em->getCurrentTarget() == node)
				{
					auto locationInNode = node->convertToNodeSpace(Vec2(em->getCursorX(), em->getCursorY()));
					auto s = node->getContentSize();
					Rect hitrect = Rect(0, 0, s.width, s.height);
					if(hitrect.containsPoint(locationInNode))
					{
						_uphandle = evmouse(em);
					}
				}
			}
		}
	};
	_dispatcher->addEventListenerWithSceneGraphPriority(elm, node);
}
void MouseEventManager::addMouseScorllEvent(Node *node, MouseEventCallBack evmouse)
{
	auto elm = EventListenerMouse::create();
	elm->onMouseScroll = [=](Event *e)
	{
		auto em = (EventMouse *)e;
		if(!_downhandle)
		{
			if(em->getCurrentTarget() == node)
			{
				auto locationInNode = node->convertToNodeSpace(Vec2(em->getCursorX(), em->getCursorY()));
				auto s = node->getContentSize();
				Rect hitrect = Rect(0, 0, s.width, s.height);
				if(hitrect.containsPoint(locationInNode))
				{
					_scrollhandle = evmouse(em);
				}
			}
		}
	};
	_dispatcher->addEventListenerWithSceneGraphPriority(elm, node);
}