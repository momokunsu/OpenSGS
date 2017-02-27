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
	public:
		static Card* create() { return new Card(); }

		void setId(ushort id) { m_id = id; }
		ushort getId() { return m_id; }

		void setType(eCardType type) { m_type = type; }
		eCardType getType() { return m_type; }

		void setName(const char* name) { m_name = name; }
		const char * getName(){ return m_name.c_str(); }

	protected:
		Card();
		virtual ~Card() {}

		ushort m_id;
		eCardType m_type;
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
