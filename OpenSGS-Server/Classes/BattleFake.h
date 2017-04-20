#pragma once

#include "BattleProcessor.h"
#include "PlayerFake.h"


class BattleFake : public BattleProcessor
{
	public:
		BattleFake(void);
		virtual ~BattleFake(void);
		
		virtual bool start();
		virtual void Process();

	private:
		void _detachPlayersPos();
		uint _startgeneralcount;
};

