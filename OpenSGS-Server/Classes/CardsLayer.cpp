#include "CardsLayer.h"

using namespace cocos2d;

CardsLayer::CardsLayer(void)
{
}
CardsLayer::~CardsLayer(void)
{
}


bool CardsLayer::init()
{
	if(!Layer::init())
		return false;

	ignoreAnchorPointForPosition(false);
	setContentSize(Size(200.0f, 281.0f));

	auto func = std::bind(&CardsLayer::init);
	//func(this);
	return true;
}

void CardsLayer::setSizeWidth(float w)
{
	setContentSize(Size(w, 281.0f));
}

void CardsLayer::addCard(Node *image, float pos)
{
	image->setAnchorPoint(Vec2(0.0f, 0.0f));
	if(getChildrenCount())
	{
		auto childs = getChildren();
		if(pos < 0)
			addChild(image, childs.back()->getLocalZOrder() + 1);
		else
			addChild(image, pos);
	}
	else
	{
		addChild(image, 0);
	}
	sortCards();
}
void CardsLayer::addCardwithAction(Node *image, float delay, float pos)
{
	image->setAnchorPoint(Vec2(0.0f, 0.0f));
}

void CardsLayer::removeCard(Node *image, float pos)
{
}
void CardsLayer::removeCardwithAction(Node *image, float delay, float pos)
{
}

void CardsLayer::sortCards()
{
	sortAllChildren();
	auto childs = getChildren();

	int i = 0;
	auto size = getContentSize();
	for(auto it : childs)
	{
		it->setPosition(size.width * i / childs.size(), 0.0f);
	}
}

void CardsLayer::sortCardswithAction(float delay)
{
}