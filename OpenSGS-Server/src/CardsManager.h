#pragma once

#include <string>
#include <map>

#include "def.h"
#include "GamePackFile.h"


class CardsManager
{
	public:
		static void addSerchPath(const char *path) { GamePackFile::addSerchPath(path); }
		static void removeAllSerchPath() { GamePackFile::removeAllSerchPath(); }
		static const std::map<std::string, GamePackFile*>& getFileCache() { GamePackFile::getFileCache(); }

		static void addPack(const char *filename, short offset = -1) { m_packname_buf[filename] = offset; }
		static void removePack(const char *filename){ m_packname_buf.erase(std::find(m_packname_buf.begin(), m_packname_buf.end(), filename)); }
		static void removeAllPack() { m_packname_buf.clear(); }
		static const std::map<std::string, short>& getPackNames() { return m_packname_buf; }

		static void initDeckToList(std::list<uint>& vec);
		static Card* getCardInfo(uint);

	private:
		static std::map<std::string, short> m_packname_buf;
		static std::map<uint, Card*> m_id_cards;
};

