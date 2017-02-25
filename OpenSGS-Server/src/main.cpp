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
	auto n = tag.c_str();
	auto str = LogHandler::getCurFileName().c_str();
	printf("[%s:%d %s]%s\n", STR::subString(str, STR::endIndexOf(str, '\\') + 1), LogHandler::getCurLineNum(), tag.c_str(), log.c_str());
}

int main()
{
	LogHandler::setLogEventCallback(Log);

	auto engine = new ScriptEngine();

	auto sys = new BattleSystem();

	for (int i = 0; i < 8; i++)
	{
		sys->addPlayer(new Player());
	}
	sys->sufflePlayersLocation();

	CardsManager::addSerchPath(".");
	CardsManager::addPack("standard.gpk");
	if (CardsManager::initDeckToList(sys->cardsDeck()))
	{
		sys->shuffleCardsDeck();
	}

	system("pause");
	return 0;
}