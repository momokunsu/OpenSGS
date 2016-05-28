#pragma once

#include "libs/GC.h"
#include "libs/lua-5.3.2/lua.hpp"
#include "def.h"

#include <map>
#include <functional>


class ScriptEngine : public GC
{
	public:
		ScriptEngine();
		~ScriptEngine();

		int luaCall(const char *funname, va_list ap);
		int luaCall(const char *funname, ...);

	private:
		uTypeUnion luaGetValue(int index);

		static std::map<std::string, std::function<void(lua_State *, va_list)>> m_push_lua_param;

		lua_State * m_lua_state;
		std::vector<uTypeUnion> m_retval_arr;
};