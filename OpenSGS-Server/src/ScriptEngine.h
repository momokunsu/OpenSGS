#pragma once

#include "libs/GC.h"
#include "libs/lua-5.3.2/lua.hpp"
#include "def.h"
#include "Player.h"

#include <map>
#include <functional>


class Script;

class ScriptEngine : public GC
{
	public:
		ScriptEngine();
		~ScriptEngine();

		bool loadScript(const char * script, const char * name = "");
		bool loadScriptFromFile(const char * filename);

		int luaCall(const char *funname, va_list ap);
		int luaCall(const char *funname, ...);

	private:
		uTypeUnion luaGetValue(int index);
		bool luaAsserts(int res, int tindex);

		static std::map<std::string, std::function<void(lua_State *, va_list)>> m_push_lua_param;

		lua_State * m_lua_state;
		std::vector<uTypeUnion> m_retval_arr;
};

class Script : public GC
{
	public:
		Script() {}
		virtual ~Script() = 0 {}

	private:
		ScriptEngine *m_runner;
};

class ScriptBaseCard : public Script
{
	friend class ScriptEngine;
	public:
		bool canUse(Player *player, int pos);
		bool canUseObjects(std::vector<Player *> &players);

	private:
		ScriptBaseCard();
		virtual ~ScriptBaseCard();
};