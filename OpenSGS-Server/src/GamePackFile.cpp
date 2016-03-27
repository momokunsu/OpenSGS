#include "libs/StringManager.h"
#include "def.h"
#include "GamePackFile.h"
#include "LogHandler.h"

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

GamePackFile::GamePackFile(const char * filename)
{
	m_db = nullptr;
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
		LogHandler::setLog("GamePackFile::loadInfo", STR::format("sqlite3: %s \nfilename: %s", sqlite3_errmsg(m_db), m_filename.c_str()));
		close();
		return false;
	}
	return true;
}

void GamePackFile::close()
{
	if (m_db)
	{
		LogHandler::setLog("GamePackFile::loadInfo", STR::format("sqlite3: close file! \nfilename: %s", sqlite3_errmsg(m_db), m_filename.c_str()));
		sqlite3_close(m_db);
		m_db = nullptr;
	}
}

bool GamePackFile::loadInfo()
{
	if (!loadPackInfo())
		return false;
}

bool GamePackFile::loadPackInfo()
{
	LogHandler::setLog("GamePackFile::loadPackInfo", STR::format("begin load pack info \nfilename: %s", m_filename.c_str()));

	sqlite3_stmt *sqlstate;

	//读取包信息
	if (sqlite3_prepare(m_db, "select * from info", -1, &sqlstate, NULL) != SQLITE_OK)
	{
		LogHandler::setLog("GamePackFile::loadPackInfo", STR::format("sqlite3: %s \nfilename: %s", sqlite3_errmsg(m_db), m_filename.c_str()));
		close();
		return false;
	}

	auto result = sqlite3_step(sqlstate);
	if (result == SQLITE_ROW)
	{
		const char *str_res = nullptr;

		if (str_res = (const char*)sqlite3_column_text(sqlstate, (int)ePackInfoIndex::Name))
			m_packinfo.packName = str_res;
		if (str_res = (const char*)sqlite3_column_text(sqlstate, (int)ePackInfoIndex::Author))
			m_packinfo.author = str_res;
		if (str_res = (const char*)sqlite3_column_text(sqlstate, (int)ePackInfoIndex::CreateDate))
			m_packinfo.createDate = str_res;
		if (str_res = (const char*)sqlite3_column_text(sqlstate, (int)ePackInfoIndex::Version))
			m_packinfo.version = str_res;
		if (str_res = (const char*)sqlite3_column_text(sqlstate, (int)ePackInfoIndex::CreateDate))
			m_packinfo.createDate = str_res;
		if (str_res = (const char*)sqlite3_column_text(sqlstate, (int)ePackInfoIndex::CreateDate))
			m_packinfo.createDate = str_res;

		int int_res = 0;
		m_packinfo.cardNum = sqlite3_column_int(sqlstate, (int)ePackInfoIndex::CardNum);
		m_packinfo.baseNum = sqlite3_column_int(sqlstate, (int)ePackInfoIndex::BaseNum);
		m_packinfo.schemeNum = sqlite3_column_int(sqlstate, (int)ePackInfoIndex::SchemeNum);
		m_packinfo.equipNum = sqlite3_column_int(sqlstate, (int)ePackInfoIndex::EquipNum);
		m_packinfo.heroNum = sqlite3_column_int(sqlstate, (int)ePackInfoIndex::HeroNum);
		m_packinfo.skillNum = sqlite3_column_int(sqlstate, (int)ePackInfoIndex::SkillNum);
	}
	else if (result != SQLITE_DONE)
	{
		LogHandler::setLog("GamePackFile::loadPackInfo", STR::format("sqlite3: %s \nfilename: %s", sqlite3_errmsg(m_db), m_filename.c_str()));
		close();
		return false;
	}

	return true;
}
