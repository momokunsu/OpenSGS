#include "libs/StringManager.h"

#include "def.h"
#include "GameEvent.h"
#include "CardsManager.h"
#include "LogHandler.h"
#include "BattleSystem.h"

typedef StringManager STR;


void Log(const std::string& tag, const std::string& log)
{
	printf("%s:  %s\n", tag.c_str(), log.c_str());
}

int main()
{
	int r1 = STR::compare("constchar", "char");
	int r2 = STR::compare("constchar", "const");
	int r3 = STR::compare("const", "constchar"); 
	int r4 = STR::compare("constchar", "constchar");
	auto r5 = STR::equals("constchar", "constchar");
	auto r6 = STR::equals("constchar", "char");

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