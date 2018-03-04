#pragma once

#include <list>
#include <set>

#include "../def.h"

class GC
{
	public:
		GC();
		virtual ~GC();

		void addRef(GC* gc);
		void removeRef(GC* gc);
		bool findRef(GC* gc);
		void retain() { addRef(&m_root_ref); }
		void release() { removeRef(&m_root_ref); }

		static void recycle();

		static void* getGlobalBuffer() { return m_global_buffer; }
		static int getGlobalBufSize();
		static bool isRangeOfGlobalBuf(const void *buf);
	protected:

		static char *m_global_buffer;
		static std::set<GC*> m_garbage_pool;
		static GC m_root_ref;

		std::set<GC*> m_ref_pool;
};

#define GC_CREATE(c) inline c* create() { auto v = new c; m_garbage_pool.insert(v); return v; }
