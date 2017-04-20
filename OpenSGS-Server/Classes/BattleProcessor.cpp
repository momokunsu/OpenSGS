#include "BattleProcessor.h"
#include <stdlib.h>

BattleProcessor::BattleProcessor(void)
{
	_isstart = false;
	//_maxpalyer = 10;
	_players.resize(10);
	_curtime = NULL;
}

BattleProcessor::~BattleProcessor(void)
{
}

bool BattleProcessor::addPlayer(Player *player, int pos)
{
	if(pos >= (int)_players.size()) return false;
	if(pos < 0)
	{
		uint i;
		for(i = 0; i < _players.size(); i++)
		{
			if(!_players[i])
			{
				_players[i] = player;
				break;
			}
		}
		if(i == _players.size())
			return false;
	}
	else
	{
		pos = (pos-1) % _players.size()+1;
		_players[pos-1] = player;
	}
	addRef(player);
	return true;
}

void BattleProcessor::setCardgroup(CardVector *group)
{
	if(!_isstart)
	{
		_cardgroup = group;
		addRef(_cardgroup);
	}
}

CmdBuffer *BattleProcessor::getResult()
{
	if(!_cmdque.size())
		return nullptr;
	auto cmd = _cmdque.front();
	_cmdque.pop();
	removeRef(cmd);
	return cmd;
}