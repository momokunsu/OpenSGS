#pragma once
#include "libs/GC.h"
#include "libs/lua-5.3.2/lua.hpp"

#include <map>
#include <functional>


class ScriptEngine : public GC
{
	public:
		ScriptEngine();
		~ScriptEngine();

		void luaCall(const char *funname, va_list ap);
		void luaCall(const char *funname, ...);

	private:
		static std::map<std::string, std::function<void(lua_State *, va_list)>> m_push_lua_param;

		lua_State * m_lua_state;
		int m_ret_index;
};