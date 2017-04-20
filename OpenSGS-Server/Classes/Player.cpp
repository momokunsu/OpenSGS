#include "Player.h"


Player::Player(io_handle_t h)
{
	//游戏相关
	_id = NULL;
	_hp = _hpmax = 0;
	_pos = 0;
	_judgecard = new CardVector();
	addRef(_judgecard);
	_equipcard[4] = _equipcard[3] = _equipcard[2] = _equipcard[1] = _equipcard[0] = nullptr;

	//网络相关
	_sock = h;
	if(!_sock)
	{
		_IPaddress.clear();
		return;
	}
	iop_sockaddr_in addr;
	int len = sizeof(addr);
	getpeername(h, (iop_sockaddr *)&addr, &len);
	//getsockname(h, (iop_sockaddr *)&addr, &len);
	_IPaddress =inet_ntoa(addr.sin_addr);
	_port = ntohs(addr.sin_port);
	_isconnect = false;
}
Player::~Player(void)
{
}


CmdBuffer *Player::popCurCmd()
{
	auto cmd = getCurCmd();
	_cmdque.pop();
	removeRef(cmd);
	return cmd;
}

void Player::pushCmd(CmdBuffer *cmd)
{
	_cmdque.push(cmd);
	addRef(cmd);
}

void Player::getEquipCard(Card* card[]) const
{
	card[0] = _equipcard[0];
	card[1] = _equipcard[1];
	card[2] = _equipcard[2];
	card[3] = _equipcard[3];
	card[4] = _equipcard[4];
}
/*
void Player::setEquipCard(Card* card[])
{
	_equipcard[0] = card[0];
	_equipcard[1] = card[1];
	_equipcard[2] = card[2];
	_equipcard[3] = card[3];
	_equipcard[4] = card[4];
}*/