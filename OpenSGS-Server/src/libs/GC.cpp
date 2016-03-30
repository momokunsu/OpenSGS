#include "GC.h"

#include <algorithm>

char *GC::m_global_buffer = new char[4 * 1024 * 1024];
std::queue<GC*> GC::m_garbage_que;

GC::GC()
{
	retain();
	release();
}

GC::~GC()
{
	removeAllRef();
}

void GC::release()
{
	m_ref_count--;
	if (m_ref_count < 1)
		m_garbage_que.push(this);
}

void GC::addRef(GC * gc)
{
	gc->retain();
	m_ref_pool.push_back(gc);
}

void GC::removeRef(GC * gc)
{
	gc->release();
	m_ref_pool.remove(gc);
}

void GC::removeAllRef()
{
	while (m_ref_pool.size() > 0)
	{
		m_ref_pool.front()->release();
		m_ref_pool.pop_front();
	}
}

void GC::recycle()
{
	while (m_garbage_que.size() > 0)
	{
		auto obj = m_garbage_que.front();
		if (obj->m_ref_count < 1)
			delete obj;
		m_garbage_que.pop();
	}
}
