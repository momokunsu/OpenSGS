#pragma once

#include "SmartPtr.h"

typedef std::function<bool (cocos2d::EventMouse *)> MouseEventCallBack;

class MouseEventManager : public SmartPtr
{
	public:
		static MouseEventManager *getInstance();

		inline void setDoubleClickConfig(cocos2d::Size range, int delay){_dclickrange = range, _dclickdelay = delay;}

		void addMouseEnterEvent(cocos2d::Node *node, MouseEventCallBack evmouse);
		void addMouseLeaveEvent(cocos2d::Node *node, MouseEventCallBack evmouse);
		void addMouseMoveEvent(cocos2d::Node *node, MouseEventCallBack evmouse);
		void addMouseUpEvent(cocos2d::Node *node, MouseEventCallBack evmouse);
		void addMouseDownEvent(cocos2d::Node *node, MouseEventCallBack evmouse);
		void addMouseDoubleClickEvent(cocos2d::Node *node, MouseEventCallBack evmouse);
		void addMouseScorllEvent(cocos2d::Node *node, MouseEventCallBack evmouse);

	private:
		typedef struct
		{
			cocos2d::Node *node;
			MouseEventCallBack evmouse;
			bool isenter;
		}EVENTSTRUCT;
		MouseEventManager(void);
		virtual ~MouseEventManager(void);

		static MouseEventManager *self;

		cocos2d::EventDispatcher *_dispatcher;
		cocos2d::Node *_eventbuttom;
		cocos2d::Size _dclickrange;
		int _dclickdelay;
		bool _enterhandle, _movehandle, _uphandle, _downhandle, _scrollhandle;
		cocos2d::Node *_whoenter;
};

