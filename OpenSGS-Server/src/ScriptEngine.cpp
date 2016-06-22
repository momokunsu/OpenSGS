#include "ScriptEngine.h"
#include "LogHandler.h"
#include "libs/StringManager.h"

#include <vector>
#include <fstream>


using namespace std;
typedef StringManager STR;


std::map<std::string, std::function<void(lua_State *, va_list *)>> ScriptEngine::m_push_lua_param;

ScriptEngine::ScriptEngine() 
{
	if (m_push_lua_param.empty())
	{
		m_push_lua_param["nil"] = [](lua_State * L, va_list *ap) { lua_pushnil(L); };
		m_push_lua_param["bool"] = [](lua_State * L, va_list *ap) { int val = va_arg((*ap), bool); lua_pushboolean(L, val); };
		m_push_lua_param["int"] = [](lua_State * L, va_list *ap) { lua_Integer val = va_arg((*ap), int); lua_pushinteger(L, val); };
		m_push_lua_param["float"] = [](lua_State * L, va_list *ap) { lua_Number val = va_arg((*ap), double); lua_pushnumber(L, val); };
		m_push_lua_param["string"] = [](lua_State * L, va_list *ap) { char * val = va_arg((*ap), char *); lua_pushlstring(L, val, strlen(val)); };
	}
	m_lua_state = luaL_newstate();
	luaopen_base(m_lua_state);

	auto buf = (char *)GC::getGlobalBuffer();
	std::ifstream t;
	int length;
	t.open("test.lua", ios::binary);      // open input file  
	t.seekg(0, std::ios::end);    // go to the end  
	length = t.tellg();           // report location (this is the length)  
	t.seekg(0, std::ios::beg);    // go back to the beginning  
	t.read(buf, length);       // read the whole file into the buffer  
	t.close();                    // close file handle

	buf[length] = 0;
	//loadScriptFromFile("test.lua");
	loadScript(buf, "fuck!");
	loadScriptFromFile("test2.lua");
	luaCall("test(bool string int)", true, "wa ha ha!!", 12830);
	luaCall("test2(bool int float)", true, 12830, 2.5);
	luaCall("A.B.C()", true, 12830, 2.5);
}

ScriptEngine::~ScriptEngine()
{
	clearRetValues();
	lua_close(m_lua_state);
}

bool ScriptEngine::loadScript(const char * script, const char * name)
{
	if (luaAsserts(luaL_loadbuffer(m_lua_state, script, strlen(script), name), lua_gettop(m_lua_state)))
		return luaAsserts(lua_pcall(m_lua_state, 0, LUA_MULTRET, 0), lua_gettop(m_lua_state));
	return false;
}

bool ScriptEngine::loadScriptFromFile(const char * filename)
{
	if (luaAsserts(luaL_loadfile(m_lua_state, filename), lua_gettop(m_lua_state)))
		return luaAsserts(lua_pcall(m_lua_state, 0, LUA_MULTRET, 0), lua_gettop(m_lua_state));
	return false;
}

bool ScriptEngine::luaCall(const char * funname, va_list ap)
{
	int ret_index = lua_gettop(m_lua_state);

	vector<string> list;
	STR::split(list, funname, ' ', ',', '(', ')', '\t', '\r', '\n', 0);

	auto it = list.begin();
	vector<string> list_ele;
	STR::split(list_ele, (*it).c_str(), '.', 0); it++;
	auto it_ele = list_ele.begin();
	auto cur_table = (*it_ele).c_str();
	lua_getglobal(m_lua_state, cur_table); it_ele++;
	while (it_ele != list_ele.end())
	{
		int cur_top = lua_gettop(m_lua_state);
		if (!lua_istable(m_lua_state, cur_top))
		{
			LogError("lua error", STR::format("element %s is not a table!", cur_table));
			lua_pop(m_lua_state, cur_top - ret_index);
			return false;
		}
		cur_table = (*it_ele).c_str();
		lua_getfield(m_lua_state, -1, (*it_ele).c_str()); it_ele++;
		lua_remove(m_lua_state, cur_top);
	}

	int count = 0;
	while (it != list.end())
	{
		m_push_lua_param[(*it).c_str()](m_lua_state, &ap); it++;
		count++;
	}

	if (luaAsserts(lua_pcall(m_lua_state, count, LUA_MULTRET, 0), ret_index))
	{
		auto cur_index = lua_gettop(m_lua_state);
		clearRetValues();
		for (int i = ret_index + 1; i <= cur_index; i++)
			m_retval_arr.push_back(luaGetValue(i));
		return true;
	}
	return false;
}

bool ScriptEngine::luaCall(const char * funname, ...)
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
				m_retval_strings.push_back(lua_tostring(m_lua_state, index));
				ret.ptrVal[0] = (void *)m_retval_strings.back().c_str();
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
