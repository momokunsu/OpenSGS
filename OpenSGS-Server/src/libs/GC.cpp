#include "GC.h"

#include <algorithm>


const int _buffer_size = 4 * 1024 * 1024; // 4MB

char *GC::m_global_buffer = new char[_buffer_size];
std::set<GC*> GC::m_garbage_pool;
GC GC::m_root_ref;

GC::GC()
{
}

GC::~GC()
{
}

void GC::addRef(GC * gc)
{
	if(!this->findRef(gc)) 
		this->m_ref_pool.insert(gc);
}

void GC::removeRef(GC * gc)
{
	this->m_ref_pool.erase(gc);
	if(this->m_ref_pool.empty())
		m_garbage_pool.insert(this);
}

bool GC::findRef(GC * gc)
{
	if (m_ref_pool.find(gc) != m_ref_pool.end())
		return true;

	for (auto i : m_ref_pool)
	{
		if (i->findRef(gc))
			return true;
	}

	return false;
}

void GC::recycle()
{
	for (auto i : m_garbage_pool)
	{
		if (i->m_ref_pool.empty())
			delete i;
	}
	m_garbage_pool.clear();
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
