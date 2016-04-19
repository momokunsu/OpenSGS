#include "Cards.h"

Card * Card::create()
{
	return nullptr;
}

BaseCard::BaseCard()
{
	setType(eCardType::Base);
}

BaseCard::~BaseCard()
{
}
