#pragma once

#include <string>
#include <vector>
#include <map>
#include <list>

#include "def.h"
#include "GamePackFile.h"


class PackManager
{
	public:
		void addPack(const char *filename);
		void removePack(const char *filename);

		void generateData();

	private:
		std::vector<std::string> m_packname_buf;
};

