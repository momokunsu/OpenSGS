#pragma once

#include "Card.h"

class GameCard : public Card
{
	public:
		GameCard(void);
		virtual ~GameCard(void);

		inline uchar getNumber() const{return _num;}
		inline void setNumber(uchar n){_num = n;}
		
		inline uchar getPattern() const{return _pattern;}
		inline void setPattern(uchar p){_pattern = p;}
		
		inline const char *getCardIDName() const{return _cardidname.c_str();}
		inline void setCardIDName(const char *idname){_cardidname = idname;}

	protected:
		std::string _cardidname;
		uchar _num;
		uchar _pattern;
};

