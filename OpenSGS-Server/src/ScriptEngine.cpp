#include "ScriptEngine.h"
#include "LogHandler.h"
#include "libs/StringManager.h"

#include <vector>


using namespace std;
typedef StringManager STR;


std::map<std::string, std::function<void(lua_State *, va_list)>> ScriptEngine::m_push_lua_param;

ScriptEngine::ScriptEngine()
{
	if (m_push_lua_param.empty())
	{
		m_push_lua_param["nil"] = [](lua_State * L, va_list ap) { lua_pushnil(L); };
		m_push_lua_param["bool"] = [](lua_State * L, va_list ap) { int val = va_arg(ap, bool); lua_pushboolean(L, val); };
		m_push_lua_param["int"] = [](lua_State * L, va_list ap) { lua_Integer val = va_arg(ap, int); lua_pushinteger(L, val); };
		m_push_lua_param["float"] = [](lua_State * L, va_list ap) { lua_Number val = va_arg(ap, double); lua_pushnumber(L, val); };
		m_push_lua_param["string"] = [](lua_State * L, va_list ap) { char * val = va_arg(ap, char *); lua_pushlstring(L, val, strlen(val)); };
	}
	m_lua_state = luaL_newstate();

	luaAsserts(luaL_dofile(m_lua_state, "test.lua"), lua_gettop(m_lua_state));
	int ret_index = lua_gettop(m_lua_state);
	luaAsserts(luaL_dofile(m_lua_state, "test2.lua"), lua_gettop(m_lua_state));
	ret_index = lua_gettop(m_lua_state);
	luaCall("test(bool int float)", true, 12830, 2.5);
	luaCall("test2(bool int float)", true, 12830, 2.5);
}

ScriptEngine::~ScriptEngine()
{
	lua_close(m_lua_state);
}

bool ScriptEngine::loadScript(const char * script)
{
	return luaAsserts(luaL_dostring(m_lua_state, script), lua_gettop(m_lua_state));
}

bool ScriptEngine::loadScriptFromFile(const char * filename)
{
	return luaAsserts(luaL_dofile(m_lua_state, filename), lua_gettop(m_lua_state));
}

int ScriptEngine::luaCall(const char * funname, va_list ap)
{
	int ret_index = lua_gettop(m_lua_state);

	vector<string> list;
	STR::split(list, funname, ' ', ',', '(', ')', '\t', '\r', '\n');

	auto it = list.begin();

	lua_getglobal(m_lua_state, (*it).c_str());          /* 将调用的函数 */
	it++;

	int count = 0;
	while (it != list.end())
	{
		m_push_lua_param[(*it).c_str()](m_lua_state, ap);
		it++;
		count++;
	}

	if (luaAsserts(lua_pcall(m_lua_state, count, LUA_MULTRET, 0), ret_index))
	{
		auto cur_index = lua_gettop(m_lua_state);
		for (int i = ret_index + 1; i <= cur_index; i++)
			m_retval_arr.push_back(luaGetValue(i));
		return true;
	}
	return false;
}

int ScriptEngine::luaCall(const char * funname, ...)
{
	va_list ap;
	va_start(ap, funname);
	auto ret = luaCall(funname, ap);
	va_end(ap);
	return ret;
}

uTypeUnion ScriptEngine::luaGetValue(int index)
{
	uTypeUnion ret;
	ret.int64Val = 0;
	switch (lua_type(m_lua_state, index))
	{
		case LUA_TNIL:
			ret.int64Val = 0;
			break;
		case LUA_TNUMBER:
			ret.doubleVal = lua_tonumber(m_lua_state, index);
			break;
		case LUA_TBOOLEAN:
			ret.intVal[0] = lua_toboolean(m_lua_state, index);
			break;
		case LUA_TSTRING:
			{
				auto src = lua_tostring(m_lua_state, index);
				int len = strlen(src);
				auto dst = new char[len + 1];

				ret.ptrVal[0] = (void *)lua_tostring(m_lua_state, index);
			}
			break;
		default:
			break;
	}
	return ret;
}

bool ScriptEngine::luaAsserts(int res, int tindex)
{
	switch (res)
	{
		case LUA_OK	:
			return true;
		case LUA_YIELD:
		case LUA_ERRRUN:
		case LUA_ERRSYNTAX:
		case LUA_ERRMEM:
		case LUA_ERRGCMM:
		case LUA_ERRERR:
		default:
			{
				int top = lua_gettop(m_lua_state);
				while (top > tindex)
				{
					LogError("lua error", lua_tolstring(m_lua_state, top, nullptr));
					lua_pop(m_lua_state, 1);
					top = lua_gettop(m_lua_state);
				}
			}
			break;
	}
	return false;
}

Script::Script()
{
}

Script::~Script()
{
}
