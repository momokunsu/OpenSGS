#include "GC.h"

#include <algorithm>


const int _buffer_size = 4 * 1024 * 1024; // 4MB

char *GC::m_global_buffer = new char[_buffer_size];
std::list<GC*> GC::m_garbage_que;

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
	for (auto it : m_garbage_que)
	{
		if (it == this)
			return;
	}
	if (m_ref_count < 1)
		m_garbage_que.push_back(this);
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
		m_garbage_que.pop_front();
	}
}

int GC::getGlobalBufSize()
{
	return _buffer_size;
}

bool GC::isRangeOfGlobalBuf(const void *buf)
{
	if (buf >= &m_global_buffer[0] && buf <= &m_global_buffer[_buffer_size - 1])
		return true;
	return false;
}
