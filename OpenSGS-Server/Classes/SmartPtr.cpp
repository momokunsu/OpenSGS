#include "SmartPtr.h"


SmartPtr::SmartPtr()
{
	autorelease();
}

SmartPtr::~SmartPtr()
{
	clearAllRef();
}

void SmartPtr::addRef(cocos2d::Ref *ptr)
{
	ptr->retain();
	_childpool.push_back(ptr);
}

void SmartPtr::removeRef(cocos2d::Ref *ptr)
{
	_childpool.remove(ptr);
	if(ptr->getReferenceCount() == 1)
		ptr->autorelease();
	else
		ptr->release();
}

void SmartPtr::clearAllRef()
{
	if(_childpool.size() != 0)
	{
		for(auto i : _childpool)
		{
			if(i->getReferenceCount() == 1)
				i->autorelease();
			else
				i->release();
		}
		_childpool.resize(0);
	}
}