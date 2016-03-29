#pragma once
#include <string>
#include <map>

#include "libs/sqlite3/sqlite3.h"
#include "Cards.h"


struct PackInfo
{
	std::string packName;
	std::string author;
	char image[128 * 128 * 4];
	char icon[300 * 420 * 4];
	std::string createDate;
	std::string version;
	int cardNum;
	int baseNum;
	int schemeNum;
	int equipNum;
	int heroNum;
	int skillNum;

	PackInfo()
	{
		memset(this, 0, sizeof(PackInfo));
	}
};

struct CardInfo
{
	uint id;
	uchar number;
	uchar pattern;

	CardInfo()
	{
		memset(this, 0, sizeof(CardInfo));
	}
};

struct BaseCardInfo
{
	uint id;
	std::string name;
	std::string script;

	BaseCardInfo()
	{
		memset(this, 0, sizeof(BaseCardInfo));
	}
};

class GamePackFile
{
	public:
		GamePackFile(const char *filename);
		virtual ~GamePackFile();

		bool open();
		void close();

		bool loadInfo();
		void loadCards(std::vector<CardInfo> &vec);

		const PackInfo& getPackInfo() { return m_packinfo; }
		const std::map<uint, BaseCardInfo>& getBaseCardInfo() { return m_base; }

	private:
		sqlite3_stmt* sqlQuery(const char* script);
		int sqlGetFirstInt(const char* table, const char* col);
		bool sqlStep(sqlite3_stmt *psqlstate);
		void sqlEnd(sqlite3_stmt *psqlstate);
		void sqlReset(sqlite3_stmt *psqlstate);

		bool loadPackInfo();
		bool loadBaseInfo();

		sqlite3 *m_db;
		std::string m_filename;

		PackInfo m_packinfo;
		std::map<uint, BaseCardInfo> m_base;
};