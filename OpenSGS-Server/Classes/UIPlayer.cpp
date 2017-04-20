#include "UIPlayer.h"
#include "MouseEventManager.h"

using namespace cocos2d;
using namespace cocos2d::ui;

UIPlayer::UIPlayer(void)
{
}
UIPlayer::~UIPlayer(void)
{
}


//UIPlayerMain
bool UIPlayerMain::init()
{
	//主玩家区域
	setAnchorPoint(Vec2(0.5f, 0.0f));
	ignoreAnchorPointForPosition(false);
	setContentSize(Size(1917.0f, 456.0f));
	//卡片区域
	auto cardveiw = Sprite::create("data/pics/cardveiw.png");
	cardveiw->setAnchorPoint(Vec2(0.0f, 0.0f));
	cardveiw->setPosition(0.0f, 0.0f);
	addChild(cardveiw, 0);
	//武将区域
	auto generalveiw = Sprite::create("data/pics/generalveiw.png");
	generalveiw->setAnchorPoint(Vec2(0.0f, 0.0f));
	generalveiw->setPosition(1420.0f, 3.0f);
	addChild(generalveiw, 10);
	//血量
	_blood = UIMainPlayerBlood::create();
	_blood->setPosition(1436.0f, 287.0f);
	addChild(_blood, 11);
	//判定区按钮
	auto idlespr = Sprite::create("data/pics/judgebutton.png");
	idlespr->setScale(0.9f);
	idlespr->retain();
	auto selectspr = Sprite::create("data/pics/judgebutton.png");
	selectspr->retain();
	auto pushspr = Sprite::create("data/pics/judgebutton.png");
	pushspr->setPosition(5.0f, -5.0f);
#ifdef WIN32
	_judgebutton = MenuItemSprite::create(idlespr, pushspr, idlespr);
	idlespr->setAnchorPoint(Vec2(0.5f, 0.5f));
	idlespr->setPosition(_judgebutton->getAnchorPointInPoints());
	MouseEventManager::getInstance()->addMouseEnterEvent(_judgebutton, [=](EventMouse *e)->bool
	{
		_judgebutton->setNormalImage(selectspr);
		return true;
	});
	MouseEventManager::getInstance()->addMouseLeaveEvent(_judgebutton, [=](EventMouse *e)->bool
	{
		_judgebutton->setNormalImage(idlespr);
		idlespr->setAnchorPoint(Vec2(0.5f, 0.5f));
		return true;
	});
#else
	_judgebutton = MenuItemSprite::create(selectspr, pushspr, idlespr);
#endif
	auto menu = Menu::createWithItem(_judgebutton);
	menu->setPosition(1255.0f, 384.0f);
	menu->setLocalZOrder(100);
	addChild(menu, 10);

	
	//确定按钮
	_okbutton = Button::create();
#ifdef WIN32
	_okbutton->loadTextures("data/pics/okbutton-idle.png", "data/pics/okbutton-push.png", "data/pics/okbutton-forbidden.png");
	MouseEventManager::getInstance()->addMouseEnterEvent(_okbutton, [=](EventMouse *e)->bool
	{
		_okbutton->loadTextureNormal("data/pics/okbutton-select.png");
		return true;
	});
	MouseEventManager::getInstance()->addMouseLeaveEvent(_okbutton, [=](EventMouse *e)->bool
	{
		_okbutton->loadTextureNormal("data/pics/okbutton-idle.png");
		return true;
	});
#else
	_okbutton->loadTextures("data/pics/okbutton-select.png", "data/pics/okbutton-push.png", "data/pics/okbutton-forbidden.png");
#endif
	_okbutton->setPosition(Vec2(1464.0f, 275.0f));
	addChild(_okbutton, 5);

	
	//取消按钮
	_cancelbutton = Button::create();
#ifdef WIN32
	_cancelbutton->loadTextures("data/pics/cancelbutton-idle.png", "data/pics/cancelbutton-push.png", "data/pics/cancelbutton-forbidden.png");
	MouseEventManager::getInstance()->addMouseEnterEvent(_cancelbutton, [=](EventMouse *e)->bool
	{
		if(_cancelbutton->isEnabled())
			_cancelbutton->loadTextureNormal("data/pics/cancelbutton-select.png");
		return true;
	});
	MouseEventManager::getInstance()->addMouseLeaveEvent(_cancelbutton, [=](EventMouse *e)->bool
	{
		if(_cancelbutton->isEnabled())
			_cancelbutton->loadTextureNormal("data/pics/cancelbutton-idle.png");
		return true;
	});
#else
	_cancelbutton->loadTextures("data/pics/cancelbutton-select.png", "data/pics/cancelbutton-push.png", "data/pics/cancelbutton-forbidden.png");
#endif
	_cancelbutton->setPosition(Vec2(1446.0f, 166.0f));
	addChild(_cancelbutton, 6);
	
	//弃牌按钮
	_throwbutton = Button::create();
#ifdef WIN32
	_throwbutton->loadTextures("data/pics/throwbutton-idle.png", "data/pics/throwbutton-push.png", "data/pics/throwbutton-forbidden.png");
	MouseEventManager::getInstance()->addMouseEnterEvent(_throwbutton, [=](EventMouse *e)->bool
	{
		_throwbutton->loadTextureNormal("data/pics/throwbutton-select.png");
		return true;
	});
	MouseEventManager::getInstance()->addMouseLeaveEvent(_throwbutton, [=](EventMouse *e)->bool
	{
		_throwbutton->loadTextureNormal("data/pics/throwbutton-idle.png");
		return true;
	});
#else
	_throwbutton->loadTextures("data/pics/throwbutton-select.png", "data/pics/throwbutton-push.png", "data/pics/throwbutton-forbidden.png");
#endif
	_throwbutton->setPosition(Vec2(1516.0f, 59.0f));
	addChild(_throwbutton, 5);

	return true;
}

//UIPlayerOther
bool UIPlayerOther::init()
{
	ignoreAnchorPointForPosition(false);
	setContentSize(Size(460.0f, 249.0f));

	//底图
	auto bg = Sprite::create("data/pics/otherplayerveiw.png");
	bg->setAnchorPoint(Vec2(0.0f, 0.0f));
	addChild(bg, 0);

	//血量
	_blood = UIOtherPlayerBlood::create();
	_blood->setPosition(13.0f, 26.0f);
	addChild(_blood, 1);
	return true;
}