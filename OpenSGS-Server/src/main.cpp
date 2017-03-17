#include "libs/StringManager.h"

#include "def.h"
#include "GameEvent.h"
#include "CardsManager.h"
#include "LogHandler.h"
#include "BattleSystem.h"
#include "ScriptEngine.h"

#include <unistd.h>

typedef StringManager STR;
//typedef char A[];
//typedef char* A;

void Log(const std::string& tag, const std::string& log)
{
	std::vector<std::string> strs;
	STR::split(strs, LogHandler::getCurFileName().c_str(), '\\', '/', 0);
	printf("[%s:%d %-28s] %s\n", strs.back().c_str(), LogHandler::getCurLineNum(), tag.c_str(), log.c_str());
}

int main()
{
  char b;
  char *a = &b;
  char **d = &a;
	LogHandler::setLogEventCallback(Log);
  getcwd((char*)GC::getGlobalBuffer(), GC::getGlobalBufSize());
  printf((char*)GC::getGlobalBuffer(),0);
	auto engine = new ScriptEngine();
	engine->loadScriptFromFile("/Users/momo-su/Documents/OpenSGS/OpenSGS-Server/test.lua");
	engine->luaCall("test(bool string int)", true, "what the fuck!", 25);

	auto size = ScriptEngine::getFileSize("/Users/momo-su/Documents/OpenSGS/OpenSGS-Server/test2.lua");

	auto sys = BattleSystem::getInstance();

	for (int i = 0; i < 8; i++)
	{
		sys->addPlayer(new Player());
	}
	sys->sufflePlayersLocation();

	CardsManager::addSerchPath("/Users/momo-su/Documents/OpenSGS/OpenSGS-Server");
	CardsManager::addPack("standard.gpk");
	if (CardsManager::initDeckToList(sys->cardsDeck()))
	{
		sys->shuffleCardsDeck();
	}
  
	system("pause");
	return 0;
}
