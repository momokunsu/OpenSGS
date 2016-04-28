#pragma once
#include "libs/GC.h"
#include "libs/lua-5.3.2/lua.hpp"


class ScriptEngine : public GC
{
	public:
		ScriptEngine();
		~ScriptEngine();

		void luaCall(const char *funname, va_list ap);
		void luaCall(const char *funname, ...);

	private:
		lua_State * m_lua_state;
};