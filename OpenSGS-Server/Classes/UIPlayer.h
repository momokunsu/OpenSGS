#pragma once

#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "Player.h"
#include "UIPlayerBlood.h"


class UIPlayer : public cocos2d::Layer
{
	public:
		virtual bool init() = 0;

		inline Player *getPlayer() const{return _player;}
		inline void setPlayer(Player *p){_player = p;}

		inline const cocos2d::Vec2 &getTargetPos() const{return _targetpos;}

	protected:
		UIPlayer(void);
		virtual ~UIPlayer(void);
		
		Player *_player;
		cocos2d::Vec2 _targetpos;
};

class UIPlayerMain : public UIPlayer
{
	public:
		virtual bool init();

		CREATE_FUNC(UIPlayerMain)

	private:
		UIPlayerMain(void){}
		virtual ~UIPlayerMain(void){}

		cocos2d::MenuItemSprite *_judgebutton;
		cocos2d::ui::Button *_okbutton;
		cocos2d::ui::Button *_cancelbutton;
		cocos2d::ui::Button *_throwbutton;

		UIMainPlayerBlood *_blood;
};

class UIPlayerOther : public UIPlayer
{
	public:
		virtual bool init();

		CREATE_FUNC(UIPlayerOther)

	private:
		UIPlayerOther(void){}
		virtual ~UIPlayerOther(void){}
		
		UIOtherPlayerBlood *_blood;
};