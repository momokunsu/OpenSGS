#include "libs/sqlite3/sqlite3.h"
#include "libs/StringManager.h"

#include "def.h"
#include "GameEvent.h"
#include "GamePackFile.h"
#include "LogHandler.h"
#include "BattleSystem.h"


void Log(const std::string& tag, const std::string& log)
{
	printf("%s:  %s\n", tag.c_str(), log.c_str());
}

int main()
{
	auto sys = new BattleSystem();

	GamePackFile::addSerchPath(".");
	auto file = GamePackFile::create("standard.gpk");
	if (file)
	{
		file->open();
		file->loadInfo();

		auto info = file->getPackInfo();

		file->loadDeckList(sys->cardsDeck());
		sys->shuffleCardsDeck();
	}

	system("pause");
	return 0;
}