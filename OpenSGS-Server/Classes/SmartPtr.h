#pragma once

#include "cocos2d.h"
#include "GameDef.h"

class SmartPtr : public cocos2d::Ref
{
	public:
		SmartPtr();
		virtual ~SmartPtr();

		void addRef(cocos2d::Ref *ptr);
		void removeRef(cocos2d::Ref *ptr);
		void clearAllRef();

	private:
		std::list<cocos2d::Ref *> _childpool;
};