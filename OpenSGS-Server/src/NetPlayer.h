#pragma once

#include "Player.h"

class NetPlayer : public Player
{
	public:
		NetPlayer();
		~NetPlayer();

		void eventCallback(GameEvent& ev) override;

	protected:
};