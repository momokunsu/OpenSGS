#include "libs/StringManager.h"
#include "def.h"
#include "GamePackFile.h"
#include "LogHandler.h"

#pragma pack(push)
#pragma pack(1)

typedef StringManager STR;

enum class ePackInfoIndex
{
	Name = 0,
	Author,
	Image,
	icon,
	CreateDate,
	Version,
	CardNum,
	BaseNum,
	SchemeNum,
	EquipNum,
	HeroNum,
	SkillNum
};

enum class ePackCardsIndex
{
	Id = 0,
	Name,
	Type,
	Depend
}; 

enum class ePackBaseIndex
{
	Id = 0,
	Script
};

enum class ePackDeckListIndex
{
	Id = 0,
	Pattern,
	Number
};

GamePackFile::GamePackFile(const char * filename)
{
	m_db = nullptr;
	m_idoffset = 0;
	m_filename = filename;
}

GamePackFile::~GamePackFile()
{
	close();
}

bool GamePackFile::open()
{
	if (sqlite3_open(m_filename.c_str(), &m_db) != SQLITE_OK)
	{
		LogHandler::setLog("GamePackFile::loadInfo", STR::format("sqlite3: %s \"%s\"", sqlite3_errmsg(m_db), m_filename.c_str()));
		close();
		return false;
	}
	return true;
}

void GamePackFile::close()
{
	if (m_db)
	{
		LogHandler::setLog("GamePackFile::close", STR::format("sqlite3: close file! \"%s\"", m_filename.c_str()));
		sqlite3_close(m_db);
		m_db = nullptr;
	}
}

sqlite3_stmt* GamePackFile::sqlQuery(const char * script)
{
	sqlite3_stmt *sqlstate = nullptr;
	if (sqlite3_prepare(m_db, script, -1, &sqlstate, NULL) != SQLITE_OK)
	{
		LogHandler::setLog("GamePackFile::loadPackInfo", STR::format("sqlite3: %s \"%s\"", sqlite3_errmsg(m_db), m_filename.c_str()));
		close();
	}
	return sqlstate;
}

int GamePackFile::sqlGetFirstInt(const char * table, const char * col)
{
	auto sqlstate = sqlQuery(STR::format("select %s from %s", col, table));
	if (!sqlstate || !sqlStep(sqlstate))
		return 0;
	auto value = sqlite3_column_int(sqlstate, 0);
	sqlEnd(sqlstate);
	return value;
}

bool GamePackFile::sqlStep(sqlite3_stmt *sqlstate)
{
	auto result = sqlite3_step(sqlstate);
	if (result == SQLITE_ROW)
	{
		return true;
	}
	else if (result != SQLITE_DONE)
	{
		LogHandler::setLog("GamePackFile::loadPackInfo", STR::format("sqlite3: %s \"%s\"", sqlite3_errmsg(m_db), m_filename.c_str()));
		close();
	}
	return false;
}

void GamePackFile::sqlEnd(sqlite3_stmt *psqlstate)
{
	if(sqlite3_finalize(psqlstate) != SQLITE_OK)
	{
		LogHandler::setLog("GamePackFile::loadPackInfo", STR::format("sqlite3: %s \"%s\"", sqlite3_errmsg(m_db), m_filename.c_str()));
	}
}

void GamePackFile::sqlReset(sqlite3_stmt * psqlstate)
{
	if (sqlite3_reset(psqlstate) != SQLITE_OK)
	{
		LogHandler::setLog("GamePackFile::loadPackInfo", STR::format("sqlite3: %s \"%s\"", sqlite3_errmsg(m_db), m_filename.c_str()));
	}
}

bool GamePackFile::loadInfo()
{
	if (!m_db)
	{
		LogHandler::setLog("GamePackFile::loadPackInfo", STR::format("not open the data file!"));
		return false;
	}

	if (!loadPackInfo())
		return false;

	if (!loadBaseInfo())
		return false;

	return true;
}

bool GamePackFile::loadPackInfo()
{
	LogHandler::setLog("GamePackFile::loadPackInfo", STR::format("begin load pack info! \"%s\"", m_filename.c_str()));

	//读取包信息
	auto sqlstate = sqlQuery("select * from info");
	if (!sqlstate || !sqlStep(sqlstate))
		return false;

	const char *str_res = nullptr;

	if (str_res = (const char*)sqlite3_column_text(sqlstate, (int)ePackInfoIndex::Name))
		m_packinfo.packName = str_res;
	if (str_res = (const char*)sqlite3_column_text(sqlstate, (int)ePackInfoIndex::Author))
		m_packinfo.author = str_res;
	if (str_res = (const char*)sqlite3_column_text(sqlstate, (int)ePackInfoIndex::CreateDate))
		m_packinfo.createDate = str_res;
	if (str_res = (const char*)sqlite3_column_text(sqlstate, (int)ePackInfoIndex::Version))
		m_packinfo.version = str_res;

	m_packinfo.cardNum = sqlite3_column_int(sqlstate, (int)ePackInfoIndex::CardNum);
	m_packinfo.baseNum = sqlite3_column_int(sqlstate, (int)ePackInfoIndex::BaseNum);
	m_packinfo.schemeNum = sqlite3_column_int(sqlstate, (int)ePackInfoIndex::SchemeNum);
	m_packinfo.equipNum = sqlite3_column_int(sqlstate, (int)ePackInfoIndex::EquipNum);
	m_packinfo.heroNum = sqlite3_column_int(sqlstate, (int)ePackInfoIndex::HeroNum);
	m_packinfo.skillNum = sqlite3_column_int(sqlstate, (int)ePackInfoIndex::SkillNum);

	sqlEnd(sqlstate);
	LogHandler::setLog("GamePackFile::loadPackInfo", STR::format("end load pack info! \"%s\"", m_filename.c_str()));
	return true;
}

bool GamePackFile::loadBaseInfo()
{
	LogHandler::setLog("GamePackFile::loadBaseInfo", STR::format("begin load base info! \"%s\"", m_filename.c_str()));

	//读取基本牌信息
	auto sqlstate = sqlQuery("select * from base");
	if (!sqlstate)
		return false;

	m_base.clear();
	while (sqlStep(sqlstate))
	{
		const char *str_res = nullptr;

		BaseCardInfo info;
		info.id = sqlite3_column_int(sqlstate, (int)ePackBaseIndex::Id) + m_idoffset;
		if (str_res = (const char*)sqlite3_column_text(sqlstate, (int)ePackBaseIndex::Script))
			info.script = str_res;

		auto sqlstate1 = sqlQuery("select name from cards");
		if (sqlstate1 && sqlStep(sqlstate1))
		{
			if (str_res = (const char*)sqlite3_column_text(sqlstate1, 0))
				info.name = str_res;
		}
		sqlEnd(sqlstate1);

		m_base[info.id] = info;
	}

	sqlEnd(sqlstate);
	LogHandler::setLog("GamePackFile::loadBaseInfo", STR::format("end load base info! \"%s\"", m_filename.c_str()));
	return true;
}

void GamePackFile::loadDeckList(std::vector<uint>& vec)
{
	LogHandler::setLog("GamePackFile::loadDeckList", STR::format("begin load decklist! \"%s\"", m_filename.c_str()));

	auto sqlstate = sqlQuery("select * from decklist");
	while (sqlStep(sqlstate))
	{
		uTypeUnion val;
		val.shortVal[0] = sqlite3_column_int(sqlstate, (int)ePackDeckListIndex::Id) + m_idoffset;
		val.charVal[2] = sqlite3_column_int(sqlstate, (int)ePackDeckListIndex::Pattern);
		val.charVal[3] = sqlite3_column_int(sqlstate, (int)ePackDeckListIndex::Number);
		vec.push_back(val.intVal[0]);
	}

	sqlEnd(sqlstate);
	LogHandler::setLog("GamePackFile::loadDeckList", STR::format("end load decklist! \"%s\"", m_filename.c_str()));
}
