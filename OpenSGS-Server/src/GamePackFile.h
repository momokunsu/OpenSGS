#pragma once
#include <string>
#include <vector>

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
	int id;
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

		const PackInfo& getPackInfo() { return m_packinfo; }

	private:
		bool loadPackInfo();
		bool loadBaseInfo();

		sqlite3 *m_db;
		std::string m_filename;

		PackInfo m_packinfo;
		std::vector<BaseCardInfo> m_base;
};