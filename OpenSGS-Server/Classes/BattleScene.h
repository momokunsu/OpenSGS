#pragma once

#include "cocos2d.h"
#include "GameServer.h"
#include "BattleFake.h"

class BattleScene : public cocos2d::Scene
{
	public:
		static BattleScene* getInstance();

		virtual bool init();
		virtual void update(float dt);
		void reset();

	private:
		BattleScene(void);
		virtual ~BattleScene(void);
		CREATE_FUNC(BattleScene)

		static BattleScene *self;
		cocos2d::Sprite *_background;
		BattleFake *_battle;
		GameServer *_sever;
};

