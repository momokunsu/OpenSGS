#pragma once

#include "Card.h"
#include "SmartPtr.h"

class CardVector : public SmartPtr
{
	public:
		CardVector(void){}
		virtual ~CardVector(void){}

		int getSize() const{return _Cards.size();}
		Card* getCard(int index);
		inline Card* getNextCard(){return *(_curit++);}
		int findCard(Card* card) const;
		int findCard(const char *name) const;

		Card* popCard(int index);
		void pushCardFront(Card* card);
		Card* popCardFront();
		void pushCardBack(Card* card);
		Card* popCardBack();
		bool insertCard(Card* card, int index);

		void shuffle();

	private:
		std::list<Card *> _Cards;
		std::list<Card *>::const_iterator _curit;
};

