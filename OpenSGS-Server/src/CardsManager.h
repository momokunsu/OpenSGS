#pragma once

#include <string>
#include <vector>
#include <map>
#include <list>

#include "def.h"

struct PackInfo
{
	std::string packType;
	std::string packName;
	std::string author;
	char image[128 * 128 * 4];
	char icon[300 * 420 * 4];
	std::string createDate;
	std::string version;
	int cardAmount;
	int baseAmount;
	int schemeAmount;
	int equipAmount;

	PackInfo()
	{
		memset(this, 0, sizeof(PackInfo));
	}
};

class CardsManager
{
	public:
		static CardsManager *getInstance();

		void getGamePack(GamePackInfoVec &vec = GamePackInfoVec());
		void addPackToCardGroup(std::string packname);
		void removePackFromCardGroup(std::string packname);
		//CardVector *createCardGroup();

		//ushort getGameCardsID(const char *name){return _cardmapid[name];}

	private:
		CardsManager(void);
		virtual ~CardsManager(void);
		static CardsManager *self;

		void _readGamePackInfo(const char* name, PackInfo &info);
		std::map<std::string, std::string> _gamepackpath, _generalpackpath;
		std::list<std::string> _cardgroupfile, _generalgroupfile;
		std::map<std::string, std::string *> _gamescript, _skillscript;
		std::map<std::string, ushort> _cardmapid;
		std::vector<std::string> _cardmapname;
};

