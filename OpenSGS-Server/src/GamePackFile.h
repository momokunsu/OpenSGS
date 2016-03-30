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

struct BaseCardInfo
{
	ushort id;
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

		short setIdoffset() { return m_idoffset; }
		void setIdoffset(short offset) { m_idoffset = offset; }

		bool loadInfo();
		void loadDeckList(std::vector<uint> &vec);

		const PackInfo& getPackInfo() { return m_packinfo; }
		const std::map<ushort, BaseCardInfo>& getBaseCardInfo() { return m_base; }

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
		short m_idoffset;

		PackInfo m_packinfo;
		std::map<ushort, BaseCardInfo> m_base;
};