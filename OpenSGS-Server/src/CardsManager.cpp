#include "CardsManager.h"

#define GAMECARDPATH "data/game/"
#define GENERALCARDPATH "data/general/"
#define GAMEPICSPATH "data/pics/game/"
#define GENERALPICSPATH "data/pics/general/"

#define XGAMECARDPATH "xdata/game/"
#define XGENERALCARDPATH "xdata/general/"
#define XGAMEPICSPATH "xdata/pics/game/"
#define XGENERALPICSPATH "xdata/pics/general/"


std::map<std::string, short> CardsManager::m_packname_buf;
std::map<uint, Card*> CardsManager::m_id_cards;

void CardsManager::initDeckToList(std::list<uint>& vec)
{
	//导入卡包
	GamePackFile::releaseFileCache();

	for (auto info : m_packname_buf)
	{
		auto file = GamePackFile::create(info.first.c_str(), info.second);
		file->loadDeckList(vec);
	}

	//加载卡牌信息
	for (auto info : GamePackFile::getFileCache())
	{
		auto pack = info.second;
		pack->loadInfo();

		for (auto card : pack->getBaseCardInfo())
		{
			m_id_cards[card.first] = BaseCard::create(card.second);
		}
	}
}

Card * CardsManager::getCardInfo(uint id)
{
	uTypeUnion val;
	val.intVal[0] = id;
	return m_id_cards[val.shortVal[0]];
}
