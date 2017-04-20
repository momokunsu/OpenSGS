#include "UIMainPlyaerBlood.h"

using namespace cocos2d;

float UIMainPlyaerBlood::_ALLROLL = 162.9f;


UIMainPlyaerBlood::UIMainPlyaerBlood(void)
{
}

UIMainPlyaerBlood::~UIMainPlyaerBlood(void)
{
}

void UIMainPlyaerBlood::_setBloodNum(int blood, int maxblood)
{
	char num[4] = {0};

	sprintf(num, "%d", blood);
	auto bloodnum = LabelAtlas::create(num, "data/pics/bloodnum.png", 33, 40, '0');

	auto blooddiv = LabelAtlas::create(":", "data/pics/bloodnum.png", 33, 40, '0');
	blooddiv->setPositionX(bloodnum->getContentSize().width - 10);
	
	memset(num, 0, sizeof(num));
	sprintf(num, "%d", maxblood);
	auto maxbloodnum = LabelAtlas::create(num, "data/pics/bloodnum.png", 33, 40, '0');
	maxbloodnum->setPositionX(bloodnum->getContentSize().width + 10);

	//加入血量信息
	_bloodinfo->removeAllChildrenWithCleanup(true);
	_bloodinfo->addChild(bloodnum);
	_bloodinfo->addChild(blooddiv);
	_bloodinfo->addChild(maxbloodnum);

	//重新Label框计算大小
	auto labelsize = Size(bloodnum->getContentSize().width + 10 + maxbloodnum->getContentSize().width, bloodnum->getContentSize().height);
	_bloodinfo->setContentSize(labelsize);
}

bool UIMainPlyaerBlood::init()
{
	if(!Layer::init())
		return false;
	setContentSize(Size(390.0f, 156.0f));

	_maxhp = _hp = 4;

	TextureCache::getInstance()->addImage("data/pics/mblood-green.png");
	TextureCache::getInstance()->addImage("data/pics/mblood-yellow.png");
	TextureCache::getInstance()->addImage("data/pics/mblood-red.png");

	_blood = Sprite::create("data/pics/mblood-green.png");
	_blood->setAnchorPoint(Vec2(0.0f, 0.0f));
	_stencil = DrawNode::create();
	Vec2 pts[4] = {Vec2(195.0f, 0.0f), Vec2(390.0f, 0.0f), Vec2(390.0f, 312.0f), Vec2(-195.0f, 120.0f)};
	for(auto &i : pts)
		i -= Vec2(195.0f, 0.0f);
	_stencil->setPosition(195.0f, 0.0f);
	_stencil->drawPolygon(pts, 4, Color4F::WHITE, 0, Color4F::WHITE);

	//加入剪切板
	_bloodclip = ClippingNode::create(_stencil);
	_bloodclip->addChild(_blood);
	addChild(_bloodclip);

	//加入血量数值
	_bloodinfo = Node::create();
	_bloodinfo->setAnchorPoint(Vec2(0.5f, 0.5f));
	_bloodinfo->setPosition(65.0f, 84.0f);
	setMaxLifePiont(100);
	setLifePiont(100);
	addChild(_bloodinfo, 2);

	auto kbel = EventListenerKeyboard::create();
	kbel->onKeyPressed = [=](EventKeyboard::KeyCode code, Event *e)
	{
		if(code == EventKeyboard::KeyCode::KEY_LEFT_ARROW)
			setLifePiont(_hp+1);
		if(code == EventKeyboard::KeyCode::KEY_RIGHT_ARROW)
			setLifePiont(_hp-1);
		if(code == EventKeyboard::KeyCode::KEY_UP_ARROW)
			setMaxLifePiont(_maxhp+1);
		if(code == EventKeyboard::KeyCode::KEY_DOWN_ARROW)
			setMaxLifePiont(_maxhp-1);
	};
	getEventDispatcher()->addEventListenerWithSceneGraphPriority(kbel, this);

	return true;
}

void UIMainPlyaerBlood::setMaxLifePiont(int p)
{
	_maxhp = p;
	//超出检测
	if(_hp > _maxhp)
		_hp = _maxhp;

	_setBloodNum(_hp, _maxhp);
	//无血或无上限
	if(_hp < 1 || _maxhp < 1)
	{
		_stencil->setRotation(180.0f);
		return;
	}

	//设置掩盖角度
	float roll;
	roll = (_maxhp - _hp) * _ALLROLL / _maxhp;
	_stencil->setRotation(roll);

	if(_blood)
	{
		_bloodclip->removeChild(_blood);
		_blood = nullptr;
	}

	//设置颜色
	if(_hp / (float)_maxhp > 0.5f)
		_blood = Sprite::create("data/pics/mblood-green.png");
	else if(_hp / (float)_maxhp <= 0.25f || _hp == 1)
		_blood = Sprite::create("data/pics/mblood-red.png");
	else
		_blood = Sprite::create("data/pics/mblood-yellow.png");

	_blood->setAnchorPoint(Vec2(0.0f, 0.0f));
	_bloodclip->addChild(_blood, 1);
}

void UIMainPlyaerBlood::setLifePiont(int p)
{
	_hp = p;
	setMaxLifePiont(_maxhp);
}