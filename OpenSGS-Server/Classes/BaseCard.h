#pragma once

#include "GameCard.h"

class BaseCard : public GameCard
{
	public:
		BaseCard(void);
		virtual ~BaseCard(void);
		
		inline ulong getPlacetime() const{return _placetime;}
		inline void setPlacetime(ulong t){_placetime = t;}

	private:
		ulong _placetime;
		std::string _placescript;
};

