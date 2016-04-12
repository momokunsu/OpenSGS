#pragma once
#include <string>

#include "libs/GC.h"


enum class eCardType
{
	None,
	Base,
	Scheme,
	Equip,
	Hero
};

class Card : public GC
{
	public:
		static Card* create(){}

		void setName(const char* name) { m_name = name; }
		const std::string& getName(){ return m_name; }

		void setType(eCardType type) { m_type = type; }
		eCardType getType() { return m_type; }

	private:
		Card();
		virtual ~Card();

		std::string m_name;
		eCardType m_type;
};

class BaseCard :public Card
{
	public:
		BaseCard();
		virtual ~BaseCard();

	private:
		std::string m_script;
};