#include "libs/sqlite3/sqlite3.h"
#include "def.h"
#include "libs/StringManager.h"

int main()
{
	auto str = StringManager::format("test string %d is %s abc %lf", 123, "fuck", 2.5);
	return 0;
}