#include "CardVector.h"


Card* CardVector::getCard(int index)
{
	if(_Cards.size() == 0 || index < 0)
		return nullptr;
	auto it = _Cards.begin();
	for(int i = 0; i < index; i++)
	{
		if(it == _Cards.cend())
			return nullptr;
		it++;
	}
	_curit = it;
	return *it;
}


int CardVector::findCard(Card* card) const
{
	auto it = _Cards.begin();
	unsigned int i = 0;
	for(; i < _Cards.size(); i++)
	{
		if(*it == card)
			break;
		it++;
	}
	if(i >= _Cards.size())
		return -1;
	else
		return (int)i;
}
int CardVector::findCard(const char *name) const
{
	auto it = _Cards.begin();
	unsigned int i = 0;
	for(; i < _Cards.size(); i++)
	{
		auto str = (*it)->getName();
		if(!strcmp(name, str))
			break;
	}
	if(i >= _Cards.size())
		return -1;
	else
		return (int)i;
}

Card* CardVector::popCard(int index)
{
	Card *card = nullptr;
	std::list<Card *>::const_iterator it;
	if(index >= 0)
	{
		it = _Cards.begin();
		for(int i = 0; i < index; i++)
		{
			if(it == _Cards.cend())
				return card;
			it++;
		}
	}
	else
	{
		index = abs(index);
		it = _Cards.end();
		for(int i = 1; i < index; i++)
		{
			if(it == _Cards.cbegin())
				return card;
			it--;
		}
	}
	card = *it;
	_Cards.erase(it);
	removeRef(card);
	return card;
}

void CardVector::pushCardFront(Card* card)
{
	_Cards.push_front(card);
	addRef(card);
}

Card* CardVector::popCardFront()
{
	Card* card = nullptr;
	if(_Cards.size() > 0)
	{
		card = _Cards.front();
		_Cards.pop_front();
		removeRef(card);
	}
	return card;
}

void CardVector::pushCardBack(Card* card)
{
	_Cards.push_back(card);
	addRef(card);
}

Card* CardVector::popCardBack()
{
	Card* card = nullptr;
	if(_Cards.size() > 0)
	{
		card = _Cards.back();
		_Cards.pop_back();
		removeRef(card);
	}
	return card;
}

bool CardVector::insertCard(Card* card, int index)
{
	int size = _Cards.size();
	if(index > size || index < -size-1)
		return false;
	if(index >= 0)
	{
		auto it = _Cards.cbegin();
		for(int i = 0; i < index; i++)
			it++;
		_Cards.insert(it, card);
	}
	else
	{
		index = abs(index);
		auto it = _Cards.cend();
		for(int i = 1; i < index; i++)
			it--;
		_Cards.insert(it, card);
	}
	addRef(card);
	return true;
}

void CardVector::shuffle()
{
	srand((unsigned int)time(NULL));

	for(int n = _Cards.size() / 2; n > 0; n--)
	{
		insertCard(popCardFront(), rand()%_Cards.size());
	}
	for(int n = _Cards.size() / 2; n > 0; n--)
	{
		insertCard(popCardBack(), rand()%_Cards.size());
	}
}
