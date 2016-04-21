#include "Cards.h"

Card::Card()
{
	m_type = eCardType::None;
}


BaseCard * BaseCard::create(BaseCardInfo & info)
{
	auto card = new BaseCard();
	card->setId(info.id);
	card->setType(eCardType::Base);
	card->setName(info.name.c_str());
	return card;
}

