#include "libs/StringManager.h"

#include "def.h"
#include "GameEvent.h"
#include "CardsManager.h"
#include "LogHandler.h"
#include "BattleSystem.h"
#include "ScriptEngine.h"

typedef StringManager STR;


void Log(const std::string& tag, const std::string& log)
{
	printf("%s:  %s\n", tag.c_str(), log.c_str());
}

int main()
{
	LogHandler::setLogEventCallback(Log);

	auto engine = new ScriptEngine();

	auto sys = new BattleSystem();

	CardsManager::addSerchPath(".");
	CardsManager::addPack("standard.gpk");
	if (CardsManager::initDeckToList(sys->cardsDeck()))
	{
		sys->shuffleCardsDeck();
	}

	system("pause");
	return 0;
}