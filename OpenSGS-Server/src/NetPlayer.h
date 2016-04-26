#pragma once

#include "Player.h"

class NetPlayer : public Player
{
	public:
		NetPlayer();
		~NetPlayer();

		void EventCallback(GameEvent* ev) override;

	protected:
};