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

		int luaCall(const char *funname, va_list ap);
		int luaCall(const char *funname, ...);

		bool getRetValToBool() { return lua_toboolean(m_lua_state, ++m_ret_index) != 0; }
		int getRetValToInt() { return (int)lua_tointeger(m_lua_state, ++m_ret_index); }
		float getRetValToFloat() { return (float)lua_tonumber(m_lua_state, ++m_ret_index); }
		const char * getRetValToString() { return lua_tostring(m_lua_state, ++m_ret_index); }

	private:
		static std::map<std::string, std::function<void(lua_State *, va_list)>> m_push_lua_param;

		lua_State * m_lua_state;
		int m_ret_index;
};