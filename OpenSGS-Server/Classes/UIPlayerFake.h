#pragma once

#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "PlayerFake.h"


class UIPlayerFake : public cocos2d::Layer
{
	public:
		virtual bool init() = 0;

		inline PlayerFake *getPlayer() const{return _player;}
		//inline void setPlayer(PlayerFake *p){_player = p;}

		inline const cocos2d::Vec2 &getTargetPos() const{return _targetpos;}

	protected:
		UIPlayerFake(void);
		virtual ~UIPlayerFake(void);

		PlayerFake *_player;
		cocos2d::Vec2 _targetpos;
};

class UIPlayerFakeMain : public UIPlayerFake
{
	public:
		virtual bool init();

		inline void setCallback(cocos2d::ccMenuCallback cb){_judgebutton->setCallback(cb);}
		
		CREATE_FUNC(UIPlayerFakeMain)
	private:
		UIPlayerFakeMain(void);
		virtual ~UIPlayerFakeMain(void);

		cocos2d::MenuItemSprite *_judgebutton;
		cocos2d::ui::Button *_okbutton;
		cocos2d::ui::Button *_cancelbutton;
		cocos2d::ui::Button *_throwbutton;
};
