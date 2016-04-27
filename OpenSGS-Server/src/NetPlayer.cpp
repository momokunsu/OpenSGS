#include "NetPlayer.h"

NetPlayer::NetPlayer()
{
}

NetPlayer::~NetPlayer()
{
}

void NetPlayer::EventCallback(GameEvent * ev)
{
	switch (ev->getEvent())
	{
		case eGameEvent::GetPlayerStatus:
			{
				auto evins = (EventGetPlayerStatus *)ev;
				m_status = evins->statusMap[m_id];
			} 
			break;
		case eGameEvent::GetCards:
			{
				auto evins = (EventGetCards *)ev;
				m_handcards.insert(m_handcards.end(), evins->cards.begin(), evins->cards.end());
			}
			break;
	}
}
