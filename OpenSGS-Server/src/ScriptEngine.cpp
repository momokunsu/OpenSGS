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

	init();
}

ScriptEngine::~ScriptEngine()
{
	clearRetValues();
	lua_close(m_lua_state);
}

void ScriptEngine::init()
{
	int top = 0;

	// 创建表
	lua_newtable(m_lua_state);
	lua_setglobal(m_lua_state, "__Battle");

	// 添加函数
	lua_getglobal(m_lua_state, "__Battle");

	lua_pushcfunction(m_lua_state, [](lua_State * L)->int { printf("what the fuck!!!"); return 0; });
	lua_setfield(m_lua_state, -2, "Fuck");
}

bool ScriptEngine::loadScript(const char * script, const char * name)
{
	if (luaAssert(luaL_loadbuffer(m_lua_state, script, strlen(script), name), lua_gettop(m_lua_state), "ScriptEngine::loadScript", __LINE__))
		return luaAssert(lua_pcall(m_lua_state, 0, LUA_MULTRET, 0), lua_gettop(m_lua_state), "ScriptEngine::loadScript", __LINE__);
	return false;
}

bool ScriptEngine::loadScriptFromFile(const char * filename)
{
	if (luaAssert(luaL_loadfile(m_lua_state, filename), lua_gettop(m_lua_state), "ScriptEngine::loadScriptFromFile", __LINE__))
		return luaAssert(lua_pcall(m_lua_state, 0, LUA_MULTRET, 0), lua_gettop(m_lua_state), "ScriptEngine::loadScriptFromFile", __LINE__);
	return false;
}

bool ScriptEngine::luaCall(const char * funname, va_list ap)
{
	int ret_index = lua_gettop(m_lua_state);

	vector<string> list;
	STR::split(list, funname, ' ', ',', '(', ')', '\t', '\r', '\n', 0);

	auto it = list.begin();
	vector<string> list_ele;
	STR::split(list_ele, it->c_str(), '.', 0); it++;
	auto it_ele = list_ele.begin();
	auto cur_table = it_ele->c_str();
	lua_getglobal(m_lua_state, cur_table); it_ele++;
	while (it_ele != list_ele.end())
	{
		int cur_top = lua_gettop(m_lua_state);
		if (!lua_istable(m_lua_state, cur_top))
		{
			LogError("ScriptEngine::luaCall", STR::format("lua error: element %s is not a table!", cur_table));
			lua_pop(m_lua_state, cur_top - ret_index);
			return false;
		}
		cur_table = it_ele->c_str();
		lua_getfield(m_lua_state, -1, it_ele->c_str()); it_ele++;
		lua_remove(m_lua_state, cur_top);
	}

	int count = 0;
	while (it != list.end())
	{
    m_push_lua_param[it->c_str()](m_lua_state, (va_list*)&ap); it++;
		count++;
	}

	if (luaAssert(lua_pcall(m_lua_state, count, LUA_MULTRET, 0), ret_index, "ScriptEngine::luaCall", __LINE__))
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

int ScriptEngine::getFileSize(const char * filePath)
{
	char* pbuf = (char*)GC::getGlobalBuffer();
	std::ifstream t;
	long long length;

	t.open(filePath, ios::binary);
	t.seekg(0, std::ios::end);
	length = t.tellg();
	t.seekg(0, std::ios::beg);
	t.read(pbuf, length);
	t.close();

	return (int)length;
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

bool ScriptEngine::luaAssert(int res, int tindex, const char* tag, int lineNum)
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
          LogHandler::setLog(tag, STR::format("lua error: %s", lua_tolstring(m_lua_state, top, nullptr)), eLogType::Error, __FILE__, lineNum);
					lua_pop(m_lua_state, 1);
					top = lua_gettop(m_lua_state);
				}
			}
			break;
	}
	return false;
}
