#pragma once
#include <string>


enum class eCardType
{
	None,
	Base,
	Scheme,
	Equip,
	Hero
};

class Card
{
	public:
		Card();
		virtual ~Card();

		void setName(const char* name) { m_name = name; }
		const std::string& getName(){ return m_name; }

		void setType(eCardType type) { m_type = type; }
		eCardType getType() { return m_type; }

	private:
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