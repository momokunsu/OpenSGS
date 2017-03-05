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

		void init();

		bool loadScript(const char * script, const char * name = "");
		bool loadScriptFromFile(const char * filename);

		bool luaCall(const char *funname, va_list ap);
		bool luaCall(const char *funname, ...);

		bool luaAddTable(const char *name, const char *parentName = "_G");
		bool luaAddFunction(const char *name, lua_CFunction func, const char *parentName = "_G");

		const std::vector<uTypeUnion>& getRetValues() { return m_retval_arr; }
		void clearRetValues() { m_retval_arr.clear(); m_retval_strings.clear(); }

		static int getFileSize(const char *filePath);

	private:
		uTypeUnion luaGetValue(int index);
		bool luaAssert(int res, int tindex, const char* tag, int lineNum);
		bool luaGetTableToStackTop(const char *tableName);

		static std::map<std::string, std::function<void(lua_State *, va_list *)>> m_push_lua_param;

		lua_State * m_lua_state;
		std::vector<uTypeUnion> m_retval_arr;
		std::vector<std::string> m_retval_strings;
};

class Script : public GC
{
	public:
		Script() {}
		virtual ~Script() = 0;

	private:
		ScriptEngine *m_runner;
};

class ScriptBaseCard : public Script
{
	friend class ScriptEngine;
	public:
		bool canUse(Player *player, int pos);
		bool canUseObjects(std::vector<Player *> &players);

		void use(Player *player);

	private:
		ScriptBaseCard();
		virtual ~ScriptBaseCard();
};
