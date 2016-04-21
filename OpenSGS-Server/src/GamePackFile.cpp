#include "GamePackFile.h"
#include "LogHandler.h"

#include "libs/StringManager.h"
#include "def.h"

typedef StringManager STR;

enum class ePackInfoIndex
{
	Name = 0,
	Author,
	Image,
	Icon,
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
	Depend,
	DependId
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


std::vector<std::string> GamePackFile::m_serch_path;

short GamePackFile::m_cur_offset = 0;
std::map<std::string, GamePackFile*> GamePackFile::m_file_cache;


void GamePackFile::releaseFileCache()
{
	for (auto it : m_file_cache)
	{
		it.second->release();
	}
	m_file_cache.clear();
	m_cur_offset = 0;
}

GamePackFile::GamePackFile(const char * filename)
{
	m_db = nullptr;
	m_idoffset = 0;
	m_filename = filename;
}

GamePackFile::~GamePackFile()
{
	close();
	m_file_cache.erase(m_file_cache.find(m_filename));
}

GamePackFile * GamePackFile::create(const char * filename, short idoffset)
{
	auto self = m_file_cache[filename];
	if(self)
		return self;

	self = new GamePackFile(filename);
	if (!self || !self->open() || !self->loadPackInfo())
	{
		LogHandler::setLog("GamePackFile::create", STR::format("create fail: \"%s\"", filename));
		self->close();
		return nullptr;
	}
	self->close();

	if (idoffset > -1)
		self->m_idoffset = idoffset;
	else
	{
		self->m_idoffset = m_cur_offset;
		m_cur_offset += self->m_packinfo.cardNum;
	}

	self->retain();
	m_file_cache[filename] = self;
	return self;
}

void GamePackFile::addSerchPath(const char * path)
{
	m_serch_path.push_back(STR::trimEnd(STR::replace(path, '\\', '/'), '/', 0));
}

bool GamePackFile::open()
{
	for (auto path : m_serch_path)
	{
		path = path + '/' + m_filename;
		FILE* file;
		if (file = fopen(path.c_str(), "r"))
		{
			fclose(file);
			if (sqlite3_open(path.c_str(), &m_db) != SQLITE_OK)
			{
				LogHandler::setLog("GamePackFile::loadInfo", STR::format("sqlite3: %s \"%s\"", sqlite3_errmsg(m_db), m_filename.c_str()));
				close();
				return false;
			}
			return true;
		}
	}

	LogHandler::setLog("GamePackFile::loadInfo", STR::format("file \"%s\" not found!!", m_filename.c_str()));
	close();
	return false;
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

bool GamePackFile::loadDeckList(std::list<uint>& vec)
{
	if (!m_db)
	{
		LogHandler::setLog("GamePackFile::loadPackInfo", STR::format("not open the data file!"));
		return false;
	}

	LogHandler::setLog("GamePackFile::loadDeckList", STR::format("begin load decklist! \"%s\"", m_filename.c_str()));

	auto sqlstate = sqlQuery("select * from decklist");
	while (sqlStep(sqlstate))
	{
		uTypeUnion val;

		int id = sqlite3_column_int(sqlstate, (int)ePackDeckListIndex::Id);
		auto sqlstate1 = sqlQuery(STR::format("select * from cards where id = %d", id));
		const char* str = nullptr;
		if (sqlStep(sqlstate1) && (str = (const char *)sqlite3_column_text(sqlstate1, (int)ePackCardsIndex::Depend)))
		{
			auto depend = create(str);
			short depend_id = sqlite3_column_int(sqlstate1, (int)ePackCardsIndex::DependId);

			if (depend_id)
				val.shortVal[0] = depend_id + depend->getIdoffset();
			else
				val.shortVal[0] = 0;
		}
		else
			val.shortVal[0] = sqlite3_column_int(sqlstate, (int)ePackDeckListIndex::Id) + m_idoffset;
		sqlEnd(sqlstate1);

		val.charVal[2] = sqlite3_column_int(sqlstate, (int)ePackDeckListIndex::Pattern);
		val.charVal[3] = sqlite3_column_int(sqlstate, (int)ePackDeckListIndex::Number);
		vec.push_back(val.intVal[0]);
	}

	sqlEnd(sqlstate);
	LogHandler::setLog("GamePackFile::loadDeckList", STR::format("end load decklist! \"%s\"", m_filename.c_str()));
}
