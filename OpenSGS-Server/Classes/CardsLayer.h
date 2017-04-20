#pragma once

#include "CardVector.h"


class CardsLayer : public cocos2d::Layer
{
	public:
		CardsLayer(void);
		virtual ~CardsLayer(void);
		
		void setSizeWidth(float w);
		void addCard(cocos2d::Node *image, float pos = -1);
		void addCardwithAction(cocos2d::Node *image, float delay, float pos = -1);

		void removeCard(cocos2d::Node *image, float pos);
		void removeCardwithAction(cocos2d::Node *image, float delay, float pos = -1);
		
		void sortCards();
		void sortCardswithAction(float delay);

		CREATE_FUNC(CardsLayer)

	protected:
		bool init();
};

