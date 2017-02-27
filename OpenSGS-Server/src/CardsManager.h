#pragma once

#include <string>
#include <map>

#include "def.h"
#include "Cards.h"


class CardsManager
{
	public:
		static void addSerchPath(const char *path) { GamePackFile::addSerchPath(path); }
		static void removeAllSerchPath() { GamePackFile::removeAllSerchPath(); }
		static const std::map<std::string, GamePackFile*>& getFileCache() { return GamePackFile::getFileCache(); }

		static void addPack(const char *filename, short offset = -1) { m_packname_buf[filename] = offset; }
		static void removePack(const char *filename){ m_packname_buf.erase(m_packname_buf.find(filename)); }
		static void removeAllPack() { m_packname_buf.clear(); }
		static const std::map<std::string, short>& getPackNames() { return m_packname_buf; }

		static bool initDeckToList(std::list<uint>& vec);
		static Card* getCardInfo(uint id);

	private:
		static std::map<std::string, short> m_packname_buf;
		static std::map<uint, Card*> m_id_cards;
};
