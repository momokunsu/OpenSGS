#pragma once
#include <list>

class GC
{
	public:
		void retain() { m_ref_count++; }
		void release();

		int getRefCount() { return m_ref_count; }

		void addRef(GC* gc);
		void removeRef(GC* gc);
		void removeAllRef();

		static void recycle();

		static void* getGlobalBuffer() { return m_global_buffer; }
		static int getGlobalBufSize();
		static bool isRangeOfGlobalBuf(const void *buf);
	protected:
		GC();
		virtual ~GC();

		static char *m_global_buffer;
		static std::list<GC*> m_garbage_que;

		int m_ref_count;
		std::list<GC*> m_ref_pool;
};