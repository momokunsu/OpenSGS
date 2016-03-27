//#include "Encoding.h"
//#include "libs/sqlite3/sqlite3.h"
//#include "CardsManager.h"
//#include "BaseCard.h"
//
//#define GAMECARDPATH "data/game/"
//#define GENERALCARDPATH "data/general/"
//#define GAMEPICSPATH "data/pics/game/"
//#define GENERALPICSPATH "data/pics/general/"
//
//#define XGAMECARDPATH "xdata/game/"
//#define XGENERALCARDPATH "xdata/general/"
//#define XGAMEPICSPATH "xdata/pics/game/"
//#define XGENERALPICSPATH "xdata/pics/general/"
//
//CardsManager *CardsManager::self = nullptr;
//
//CardsManager::CardsManager(void)
//{
//	if(_cardmapname.size() == 0)
//		_cardmapname.push_back("");
//}
//
//
//CardsManager::~CardsManager(void)
//{
//	self = nullptr;
//}
//
//
//CardsManager *CardsManager::getInstance()
//{
//	if(!self)
//	{
//		self = new CardsManager();
//		self->retain();
//	}
//	return self;
//}
//
//void CardsManager::getGamePack(GamePackInfoVec &vec)
//{
//#ifdef WIN32
//	WIN32_FIND_DATAA filedata;
//	PackInfo info;
//	std::string path;
//	HANDLE h;
//
//	_gamepackpath.clear();
//	//主目录
//	path = GAMECARDPATH;
//	memset(&filedata, 0, sizeof(filedata));
//	h = FindFirstFileA((path+std::string("*.epk")).c_str(), &filedata);
//	if(filedata.cFileName[0])
//	{
//		_readGamePackInfo((path+filedata.cFileName).c_str(), info);
//		info.packName = std::string("O/game/") + filedata.cFileName + " " + info.packName;
//		vec.push_back(info);
//		_gamepackpath[info.packName] = path+filedata.cFileName;
//		while(FindNextFileA(h, &filedata))
//		{
//			_readGamePackInfo((path+filedata.cFileName).c_str(), info);
//			info.packName = std::string("O/game/") + filedata.cFileName + " " + info.packName;
//			vec.push_back(info);
//			_gamepackpath[info.packName] = path+filedata.cFileName;
//		}
//	}
//
//	//额外目录
//	path = XGAMECARDPATH;
//	memset(&filedata, 0, sizeof(filedata));
//	h = FindFirstFileA((path+std::string("*.epk")).c_str(), &filedata);
//	if(filedata.cFileName[0])
//	{
//		_readGamePackInfo((path+filedata.cFileName).c_str(), info);
//		info.packName = std::string("U/game/") + filedata.cFileName + " " + info.packName;
//		vec.push_back(info);
//		_gamepackpath[info.packName] = path+filedata.cFileName;
//		while(FindNextFileA(h, &filedata))
//		{
//			_readGamePackInfo((path+filedata.cFileName).c_str(), info);
//			info.packName = std::string("U/game/") + filedata.cFileName + " " + info.packName;
//			vec.push_back(info);
//			_gamepackpath[info.packName] = path+filedata.cFileName;
//		}
//	}
//#else
//	/*DIR *dirptr=NULL;
//	int i=1;
//	struct dirent *entry;
//	if((dirptr = opendir("test.dir"))==NULL)
//	{
//		//printf("opendir failed!");
//		return 1;
//	}
//	else
//	{
//		while(entry=readdir(dirptr))
//		{
//			//printf("filename%d=%s\n",i,entry->d_name);
//			i++;
//		}
//		closedir(dirptr);
//	}*/
//#endif
//}
//
//void CardsManager::addPackToCardGroup(std::string packname)
//{
//	_cardgroupfile.push_back(/*_gamepackpath[*/packname/*]*/);
//}
//
//void CardsManager::removePackFromCardGroup(std::string packname)
//{
//	_cardgroupfile.remove(/*_gamepackpath[*/packname/*]*/);
//}
//
//CardVector *CardsManager::createCardGroup()
//{
//	auto cg = new CardVector();
//
//	for(auto filename : _cardgroupfile)
//	{
//		char packtype = filename.at(0);
//		filename = _gamepackpath[filename];
//		sqlite3 *sql;
//		if(sqlite3_open(filename.c_str(), &sql) != SQLITE_OK)
//		{
//			cocos2d::log(UTF8("数据库读取错误！文件：%s"), filename.c_str());
//			sqlite3_close(sql);
//			return nullptr;
//		}
//
//		sqlite3_stmt *sqlstate;
//		sqlite3_prepare(sql, "select * from info", -1, &sqlstate, NULL);
//		if(sqlite3_step(sqlstate) == SQLITE_ROW)
//		{
//			std::string type = (const char*)sqlite3_column_text(sqlstate, 0);
//			if(type != "game")
//			{
//				cocos2d::log(UTF8("非法游戏牌包！文件：%s"), filename.c_str());
//				sqlite3_close(sql);
//				return nullptr;
//			}
//
//			int baseamount = sqlite3_column_int(sqlstate, 8);
//			int schemeamount = sqlite3_column_int(sqlstate, 9);
//			int equipamount = sqlite3_column_int(sqlstate, 10);
//			sqlite3_finalize(sqlstate);
//
//			//载入基本牌
//			sqlite3_prepare(sql, "select * from base", -1, &sqlstate, NULL);
//			while(sqlite3_step(sqlstate) == SQLITE_ROW)
//			{
//				//获取基本信息
//				const char *str = nullptr;
//				std::string id;
//				std::string name;
//				str = (const char *)sqlite3_column_text(sqlstate, 0);
//				if(str)
//					id = str;
//				str = (const char *)sqlite3_column_text(sqlstate, 1);
//				if(str)
//					name = str;
//
//				auto mapid = ' ' + id;
//				mapid = packtype + mapid;
//				auto &key = _cardmapid[mapid];
//				if(key == 0)
//				{
//					_cardmapname.push_back(id);
//					key  = _cardmapname.size() - 1;
//				}
//				auto &script = _gamescript[id];
//				if(script == nullptr)
//				{
//					str = (const char *)sqlite3_column_text(sqlstate, 2);
//					if(str)
//					{
//						script = new std::string(str);
//					}
//				}
//
//				//获取牌型
//				sqlite3_stmt *sqlstate2;
//				sqlite3_prepare(sql, "select * from list where id=?", -1, &sqlstate2, NULL);
//				sqlite3_bind_text(sqlstate2, 1, id.c_str(), -1, NULL);
//				while(sqlite3_step(sqlstate2) == SQLITE_ROW)
//				{
//					BaseCard *card = new BaseCard();
//					card->setType(eCardType::Base);
//					card->setCardIDName(id.c_str());
//					card->setName(name.c_str());
//					card->setPattern(sqlite3_column_int(sqlstate2, 1));
//					card->setNumber(sqlite3_column_int(sqlstate2, 2));
//					card->setScript(script);
//					cg->pushCardBack(card);
//				}
//				sqlite3_finalize(sqlstate2); 
//			}
//			sqlite3_finalize(sqlstate);
//		}
//		else
//		{
//			cocos2d::log(UTF8("非法游戏牌包！文件：%s"), filename.c_str());
//			sqlite3_close(sql);
//			return nullptr;
//		}
//	}
//	return cg;
//}
//
//void CardsManager::_readGamePackInfo(const char* name, PackInfo &info)
//{
//	sqlite3 *sql;
//	if(sqlite3_open(name, &sql) != SQLITE_OK)
//	{
//		cocos2d::log(UTF8("数据库读取错误！文件：%s"), name);
//		return;
//	}
//
//	sqlite3_stmt *sqlstate;
//	sqlite3_prepare(sql, "select * from info", -1, &sqlstate, NULL);
//	if(sqlite3_step(sqlstate) == SQLITE_ROW)
//	{
//		std::string type = (const char*)sqlite3_column_text(sqlstate, 0);
//		if(type != "game")
//		{
//			cocos2d::log(UTF8("非法游戏牌包！文件：%s"), name);
//			return;
//		}
//		info.packType = type;
//		info.packName = (const char*)sqlite3_column_text(sqlstate, 1);
//		const char *str = (const char*)sqlite3_column_text(sqlstate, 2);
//		if(str) info.author = str;
//		const void *data = sqlite3_column_blob(sqlstate, 3);
//		if(data)
//		{
//			auto tex = new cocos2d::Texture2D();
//			tex->initWithData(data, 480 * 270 * 24,
//				cocos2d::Texture2D::PixelFormat::RGB888, 480, 270, cocos2d::Size(480.0f, 270.0f));
//			tex->autorelease();
//			info.image = cocos2d::Sprite::createWithTexture(tex);
//		}
//		else
//			info.image = nullptr;
//		data = sqlite3_column_blob(sqlstate, 4);
//		if(data)
//		{
//			auto tex = new cocos2d::Texture2D();
//			tex->initWithData(data, 64 * 64 * 32,
//				cocos2d::Texture2D::PixelFormat::RGBA8888, 64, 64, cocos2d::Size(64.0f, 64.0f));
//			tex->autorelease();
//			info.icon = cocos2d::Sprite::createWithTexture(tex);
//		}
//		else
//			info.image = nullptr;
//		str = (const char*)sqlite3_column_text(sqlstate, 5);
//		if(str) info.createDate = str;
//		str = (const char*)sqlite3_column_text(sqlstate, 6);
//		if(str) info.version = str;
//		info.cardAmount = sqlite3_column_int(sqlstate, 7);
//		info.baseAmount = sqlite3_column_int(sqlstate, 8);
//		info.schemeAmount = sqlite3_column_int(sqlstate, 9);
//		info.equipAmount = sqlite3_column_int(sqlstate, 10);
//	}
//	
//	sqlite3_finalize(sqlstate);
//	sqlite3_close(sql);
//}