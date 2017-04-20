#include "UIPlayerBlood.h"
#include "ui/CocosGUI.h"

using namespace cocos2d;
using namespace cocos2d::ui;

//UIMainPlayerBlood
Sprite *_createblood(const char *filename)
{
	auto spr = Sprite::create(filename);
	spr->retain();
	spr->setAnchorPoint(Vec2(0.0f, 0.0f));
	return spr;
}

float UIMainPlayerBlood::_ALLROLL = 162.9f;
Sprite *UIMainPlayerBlood::_greenblood = nullptr;
Sprite *UIMainPlayerBlood::_yellowblood = nullptr;
Sprite *UIMainPlayerBlood::_redblood = nullptr;


UIMainPlayerBlood::UIMainPlayerBlood(void)
{
	if(!_greenblood)
		_greenblood = _createblood("data/pics/mblood-green.png");
	if(!_yellowblood)
		_yellowblood = _createblood("data/pics/mblood-yellow.png");
	if(!_redblood)
		_redblood = _createblood("data/pics/mblood-red.png");
}

UIMainPlayerBlood::~UIMainPlayerBlood(void)
{
}

void UIMainPlayerBlood::_setBloodNum(int blood, int maxblood)
{
	char num[8] = {0};

	sprintf(num, "%d", blood);
	if(num[0] == '-')
		num[0] = '9' + 2;
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

bool UIMainPlayerBlood::init()
{
	if(!UIPlayerBlood::init())
		return false;
	setContentSize(Size(390.0f, 156.0f));

	_maxhp = _hp = 4;
	
	//设置蒙版
	_stencil = DrawNode::create();
	Vec2 pts[4] = {Vec2(195.0f, 0.0f), Vec2(390.0f, 0.0f), Vec2(390.0f, 312.0f), Vec2(-195.0f, 120.0f)};
	for(auto &i : pts)
		i -= Vec2(195.0f, 0.0f);
	_stencil->setPosition(195.0f, 0.0f);
	_stencil->drawPolygon(pts, 4, Color4F::WHITE, 0, Color4F::WHITE);

	//加入剪切板
	_bloodclip = ClippingNode::create(_stencil);
	_bloodclip->addChild(_greenblood);
	addChild(_bloodclip);

	//加入血量数值
	_bloodinfo = Node::create();
	_bloodinfo->setAnchorPoint(Vec2(0.5f, 0.5f));
	_bloodinfo->setPosition(65.0f, 84.0f);
	setMaxLifePiont(4);
	setLifePiont(4);
	addChild(_bloodinfo);

	return true;
}

void UIMainPlayerBlood::setMaxLifePiont(int p)
{
	if(p < 0) return;
	_maxhp = p;

	//超出检测
	if(_hp > _maxhp) _hp = _maxhp;

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

	//设置颜色
	Sprite *blood = (Sprite*)_bloodclip->getChildren().at(0);
	if(_hp / (float)_maxhp <= 0.25f || (_hp == 1 && _hp != _maxhp))
	{
		if(blood == _redblood) return;
		blood = _redblood;
	}
	else if(_hp / (float)_maxhp <= 0.6f || (_hp == 2 && _hp != _maxhp))
	{
		if(blood == _yellowblood) return;
		blood = _yellowblood;
	}
	else
	{
		if(blood == _greenblood) return;
		blood = _greenblood;
	}

	_bloodclip->removeAllChildren();
	_bloodclip->addChild(blood, 1);
}

void UIMainPlayerBlood::setLifePiont(int p)
{
	_hp = p;
	setMaxLifePiont(_maxhp);
}


//UIOtherPlayerBlood

Sprite *UIOtherPlayerBlood::_greenblood = nullptr;
Sprite *UIOtherPlayerBlood::_yellowblood = nullptr;
Sprite *UIOtherPlayerBlood::_redblood = nullptr;

UIOtherPlayerBlood::UIOtherPlayerBlood(void)
{
	if(!_greenblood)
		_greenblood = _createblood("data/pics/oblood-green.png");
	if(!_yellowblood)
		_yellowblood = _createblood("data/pics/oblood-yellow.png");
	if(!_redblood)
		_redblood = _createblood("data/pics/oblood-red.png");
}

UIOtherPlayerBlood::~UIOtherPlayerBlood(void)
{
}

bool UIOtherPlayerBlood::init()
{
	if(!UIPlayerBlood::init())
		return false;
	setContentSize(Size(30.0f, 211.0f));

	_maxhp = _hp = 4;

	//设置蒙版
	_stencil = DrawNode::create();
	_stencil->drawSolidRect(Vec2(0.0f, 0.0f), _greenblood->getContentSize(), Color4F::WHITE);
	
	//加入剪切板
	_bloodclip = ClippingNode::create(_stencil);
	_bloodclip->addChild(_greenblood);
	addChild(_bloodclip);

	//加入血量数值
	auto layout = Layout::create();
	layout->setLayoutType(LayoutType::VERTICAL);
	_bloodinfo = layout;
	_bloodinfo->setContentSize(Size(10.0f, 40.0f * 3));
	_bloodinfo->setAnchorPoint(Vec2(0.5f, 0.5f));
	_bloodinfo->setPosition(this->getContentSize() / 2);
	setMaxLifePiont(4);
	setLifePiont(4);
	addChild(_bloodinfo);
	
	auto kbel = EventListenerKeyboard::create();
	kbel->onKeyPressed = [=](EventKeyboard::KeyCode code, Event *e)
	{
		if(code == EventKeyboard::KeyCode::KEY_LEFT_ARROW)
			addMaxLifePiont();
		if(code == EventKeyboard::KeyCode::KEY_RIGHT_ARROW)
			subMaxLifePiont();
		if(code == EventKeyboard::KeyCode::KEY_UP_ARROW)
			addLifePiont();
		if(code == EventKeyboard::KeyCode::KEY_DOWN_ARROW)
			subLifePiont();
	};
	getEventDispatcher()->addEventListenerWithSceneGraphPriority(kbel, this);

	return true;
}

void UIOtherPlayerBlood::_setBloodNum(int blood, int maxblood)
{
	char num[8] = {0};

	sprintf(num, "%d", blood);
	if(num[0] == '-')
		num[0] = '9' + 2;
	auto bloodnum = TextAtlas::create(num, "data/pics/bloodnum.png", 33, 40, "0");

	auto blooddiv = TextAtlas::create(":", "data/pics/bloodnum.png", 33, 40, "0");

	memset(num, 0, sizeof(num));
	sprintf(num, "%d", maxblood);
	auto maxbloodnum = TextAtlas::create(num, "data/pics/bloodnum.png", 33, 40, "0");

	//设置居中对齐
	LinearLayoutParameter* lp = LinearLayoutParameter::create();
	lp->setGravity(LinearLayoutParameter::LinearGravity::CENTER_HORIZONTAL);
	bloodnum->setLayoutParameter(lp);
	blooddiv->setLayoutParameter(lp);
	maxbloodnum->setLayoutParameter(lp);

	//重新布局
	_bloodinfo->removeAllChildren();
	_bloodinfo->addChild(bloodnum);
	_bloodinfo->addChild(blooddiv);
	_bloodinfo->addChild(maxbloodnum);
}

void UIOtherPlayerBlood::setMaxLifePiont(int p)
{
	if(p < 0) return;
	_maxhp = p;
	
	//超出检测
	if(_hp > _maxhp) _hp = _maxhp;

	_setBloodNum(_hp, _maxhp);
	//无血或无上限
	if(_hp < 1 || _maxhp < 1)
	{
		_stencil->setPosition(0.0f, - this->getContentSize().height);
		return;
	}

	//设置掩盖范围
	float roll;
	roll = (_maxhp - _hp) * this->getContentSize().height / (float)_maxhp;
	_stencil->setPosition(0.0f, -roll);

	//设置颜色
	Sprite *blood = (Sprite*)_bloodclip->getChildren().at(0);
	if(_hp / (float)_maxhp <= 0.25f || (_hp == 1 && _hp != _maxhp))
	{
		if(blood == _redblood) return;
		blood = _redblood;
	}
	else if(_hp / (float)_maxhp <= 0.6f || (_hp == 2 && _hp != _maxhp))
	{
		if(blood == _yellowblood) return;
		blood = _yellowblood;
	}
	else
	{
		if(blood == _greenblood) return;
		blood = _greenblood;
	}

	_bloodclip->removeAllChildren();
	_bloodclip->addChild(blood, 1);
}

void UIOtherPlayerBlood::setLifePiont(int p)
{
	_hp = p;
	setMaxLifePiont(_maxhp);
}