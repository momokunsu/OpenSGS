#pragma once

#include "FakeModeDef.h"
#include "Player.h"

class BattleFake;

class PlayerFake : public Player
{
	friend class BattleFake;
	public:
		PlayerFake(io_handle_t h);
		virtual ~PlayerFake(void);

		/*inline void kill(){_isdead = true; _hp = 0;}
		inline void reborn(){_isdead = false; _hp = _hpmax;}*/

	private:
		uchar _fakestatus;
		bool _isdead;
};

