#pragma once
#include <string>

#include "libs/GC.h"
#include "GamePackFile.h"


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
	Visitor(Id, m_id, ushort)
	Visitor(Type, m_type, eCardType)

	public:
		static Card* create() { return new Card(); }

		void setName(const char* name) { m_name = name; }
		const char * getName(){ return m_name.c_str(); }

	protected:
		Card();
		virtual ~Card() {}

		//ushort m_id;
		//eCardType m_type;
		std::string m_name;
};

class BaseCard : public Card
{
	public:
		static BaseCard* create(BaseCardInfo &info = c_card_info);

		void setScript(const char* script) { m_script = script; }
		const char * getScript() { return m_script.c_str(); }

	private:
		BaseCard() {}
		virtual ~BaseCard() {}
        static BaseCardInfo c_card_info;

		std::string m_script;
};
