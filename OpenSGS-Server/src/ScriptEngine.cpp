#include "ScriptEngine.h"
#include "libs/StringManager.h"

#include <vector>


using namespace std;
typedef StringManager STR;


ScriptEngine::ScriptEngine()
{
	m_lua_state = luaL_newstate();
}

ScriptEngine::~ScriptEngine()
{
	lua_close(m_lua_state);
}

void ScriptEngine::luaCall(const char * funname, va_list ap)
{
	vector<string> list;
	STR::split(funname, list, ' ', '\t', '\r', '\n', '(', ')');
}

void ScriptEngine::luaCall(const char * funname, ...)
{
	va_list ap;
	va_start(ap, funname);
	luaCall(funname, ap);
	va_end(ap);
}
