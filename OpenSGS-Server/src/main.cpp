#include "libs/sqlite3/sqlite3.h"
#include "libs/StringManager.h"

#include "def.h"
#include "GameEvent.h"

int main()
{
	auto str = StringManager::format("test string %d is %s abc %lf", 123, "fuck", 2.5);
	auto ev = new EventGetPlayerStatus();
	ev->statusMap[1] = ePlayerStatusType::Rebel;
	ev->statusMap[2] = ePlayerStatusType::Rebel;
	ev->statusMap[3] = ePlayerStatusType::Spy;
	ev->statusMap[4] = ePlayerStatusType::Subject;
	ev->statusMap[5] = ePlayerStatusType::Ruler;

	auto ev1 = new EventGetPlayerStatus();
	ev1->unserialize(ev->serialize());
	return 0;
}