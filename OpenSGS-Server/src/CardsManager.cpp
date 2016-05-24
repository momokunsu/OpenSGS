#include "CardsManager.h"
#include "libs/StringManager.h"
#include "LogHandler.h"

#define GAMECARDPATH "data/game/"
#define GENERALCARDPATH "data/general/"
#define GAMEPICSPATH "data/pics/game/"
#define GENERALPICSPATH "data/pics/general/"

#define XGAMECARDPATH "xdata/game/"
#define XGENERALCARDPATH "xdata/general/"
#define XGAMEPICSPATH "xdata/pics/game/"
#define XGENERALPICSPATH "xdata/pics/general/"

typedef StringManager STR;

std::map<std::string, short> CardsManager::m_packname_buf;
std::map<uint, Card*> CardsManager::m_id_cards;

bool CardsManager::initDeckToList(std::list<uint>& vec)
{
	//导入卡包
	GamePackFile::releaseFileCache();

	for (auto info : m_packname_buf)
	{
		auto name = info.first.c_str();
		auto file = GamePackFile::create(name, info.second);
		if (!file)
		{
			LogError("CardsManager::initDeckToList", STR::format("GamePackFile create fail: \"%s\"", name));
			return false;
		}
		file->open();
		file->loadDeckList(vec);
		file->close();
	}

	//加载卡牌信息
	for (auto info : GamePackFile::getFileCache())
	{
		auto pack = info.second;
		pack->open();
		pack->loadInfo();
		pack->close();

		for (auto card : pack->getBaseCardInfo())
		{
			m_id_cards[card.first] = BaseCard::create(card.second);
		}
	}

	return true;
}

Card * CardsManager::getCardInfo(uint id)
{
	uTypeUnion val;
	val.intVal[0] = id;
	return m_id_cards[val.shortVal[0]];
}
