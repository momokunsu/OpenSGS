#pragma once

#include "SmartPtr.h"
#include "CardVector.h"

typedef struct
{
	std::string packType;
	std::string packName;
	std::string author;
	cocos2d::Sprite *image;
	cocos2d::Sprite *icon;
	std::string createDate;
	std::string version;
	int cardAmount;
	int baseAmount;
	int schemeAmount;
	int equipAmount;
}GAMEPACKINFO;

typedef std::vector<GAMEPACKINFO> GamePackInfoVec;

class CardsManager : public SmartPtr
{
	public:
		static CardsManager *getInstance();

		void getGamePack(GamePackInfoVec &vec = GamePackInfoVec());
		void addPackToCardGroup(std::string packname);
		void removePackFromCardGroup(std::string packname);
		CardVector *createCardGroup();

		ushort getGameCardsID(const char *name){return _cardmapid[name];}

	private:
		CardsManager(void);
		virtual ~CardsManager(void);
		static CardsManager *self;

		void _readGamePackInfo(const char* name, GAMEPACKINFO &info);
		std::map<std::string, std::string> _gamepackpath, _generalpackpath;
		std::list<std::string> _cardgroupfile, _generalgroupfile;
		std::map<std::string, std::string *> _gamescript, _skillscript;
		std::map<std::string, ushort> _cardmapid;
		std::vector<std::string> _cardmapname;
};

