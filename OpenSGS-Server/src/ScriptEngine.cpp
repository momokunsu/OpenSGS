#include "ScriptEngine.h"
#include "libs/StringManager.h"

#include <vector>


using namespace std;
typedef StringManager STR;


ScriptEngine::ScriptEngine()
{
	if (m_push_lua_param.empty())
	{
		m_push_lua_param["nil"] = [](lua_State * L, va_list ap) { lua_pushnil(L); };
		m_push_lua_param["bool"] = [](lua_State * L, va_list ap) { int val = va_arg(ap, bool); lua_pushboolean(L, val); };
		m_push_lua_param["int"] = [](lua_State * L, va_list ap) { lua_Integer val = va_arg(ap, int); lua_pushinteger(L, val); };
		m_push_lua_param["float"] = [](lua_State * L, va_list ap) { lua_Number val = va_arg(ap, float); lua_pushnumber(L, val); };
		m_push_lua_param["string"] = [](lua_State * L, va_list ap) { char * val = va_arg(ap, char *); lua_pushlstring(L, val, strlen(val)); };
	}
	m_lua_state = luaL_newstate();
}

ScriptEngine::~ScriptEngine()
{
	lua_close(m_lua_state);
}

void ScriptEngine::luaCall(const char * funname, va_list ap)
{
	m_ret_index = lua_gettop(m_lua_state);

	vector<string> list;
	STR::split(funname, list, ' ', ',', '(', ')', '\t', '\r', '\n');

	auto it = list.begin();

	lua_getfield(m_lua_state, LUA_RIDX_GLOBALS, (*it).c_str());          /* 将调用的函数 */
	it++;

	int count = 0;
	while (it != list.end())
	{
		m_push_lua_param[(*it).c_str()](m_lua_state, ap);
		it++;
		count++;
	}
	lua_call(m_lua_state, count, LUA_MULTRET);
}

void ScriptEngine::luaCall(const char * funname, ...)
{
	va_list ap;
	va_start(ap, funname);
	luaCall(funname, ap);
	va_end(ap);
}
