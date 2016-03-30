#pragma once
#include <queue>
#include <list>

class GC
{
	public:
		GC();
		virtual ~GC();
		
		void retain() { m_ref_count++; }
		void release();

		int getRefCount() { return m_ref_count; }

		void addRef(GC* gc);
		void removeRef(GC* gc);
		void removeAllRef();

		static void recycle();
		static char* getGlobalBuffer() { return m_global_buffer; }
	private:
		static char *m_global_buffer;
		static std::queue<GC*> m_garbage_que;

		int m_ref_count;
		std::list<GC*> m_ref_pool;
};