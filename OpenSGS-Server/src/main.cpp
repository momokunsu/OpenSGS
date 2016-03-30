#include "libs/sqlite3/sqlite3.h"
#include "libs/StringManager.h"

#include "def.h"
#include "GameEvent.h"
#include "GamePackFile.h"
#include "LogHandler.h"

void Log(const std::string& tag, const std::string& log)
{
	printf("%s:  %s\n", tag.c_str(), log.c_str());
}

int main()
{
	LogHandler::setLogEventCallback(Log);
	auto ev = new EventGetPlayerStatus();
	ev->statusMap[1] = ePlayerStatusType::Rebel;
	ev->statusMap[2] = ePlayerStatusType::Rebel;
	ev->statusMap[3] = ePlayerStatusType::Spy;
	ev->statusMap[4] = ePlayerStatusType::Subject;
	ev->statusMap[5] = ePlayerStatusType::Ruler;

	auto ev1 = new EventGetCards();
	ev1->playerID = 10;
	ev1->cards.push_back(1);
	ev1->cards.push_back(0);
	ev1->cards.push_back(0);
	ev1->cards.push_back(8);
	ev1->cards.push_back(6);

	auto ev2 = new EventPhrase();
	ev2->playerID = 10;
	ev2->type = ePhraseType::Battle;

	auto pack = new EventsPack();
	pack->events.push_back(ev);
	pack->events.push_back(ev1);
	pack->events.push_back(ev2);

	auto pack1 = new EventsPack();
	pack1->unserialize(pack->serialize());

	auto file = new GamePackFile("standard.gpk");
	file->open();
	file->loadInfo();

	auto info = file->getPackInfo();

	std::vector<uint> vec;
	file->loadDeckList(vec);
	system("pause");
	return 0;
}