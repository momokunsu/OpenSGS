#include "BattleFake.h"
#include "CardsManager.h"
#include "PlayerFake.h"
#include "BaseCard.h"


BattleFake::BattleFake(void)
{
}

BattleFake::~BattleFake(void)
{
}


bool BattleFake::start()
{
	if(_isstart)
		return false;
	if(!_cardgroup)
		return false;
	if(_players.size() < 4)
		return false;
	
	//打乱玩家顺序
	srand((unsigned int)time(NULL));
	for(int i = 0; i < 20; i++)
	{
		int a = rand()%_players.size();
		int b = rand()%_players.size();
		auto t = _players[a];
		_players[a] = _players[b];
		_players[b] = t;
	}

	//初始化玩家信息
	int n = 0;
	for(int i = 0; i < (int)_players.size(); i++)
	{
		PlayerFake *player = (PlayerFake *)_players[i];
		if(!player)
			continue;
		player->_pos = n;
		player->_hp = player->_hpmax = 4;
		player->_fakestatus = FAKE_THIEF;
		n++;
	}
	auto buffer = new CmdBuffer();
	buffer->setCmdCode(CMD_PLAYERINIT);
	buffer->setDataLenForPos();
	_cmdque.push(buffer);
	addRef(buffer);

	//把玩家加入战斗列表
	for(int i = 0; i < (int)_players.size(); i++)
		if(_players[i])
			_lifeplayers.push_back(_players[i]);

	for(int i = 0; i < 2; i++)
	{
		auto it = _lifeplayers.begin();
		for(;it != _lifeplayers.end(); it++)
		{
			Card *card;
			auto buffer = new CmdBuffer();
			buffer->setCmdCode(CMD_DRAWCARD);
			buffer->writeByte(2);

			card = _cardgroup->popCardFront();
			(*it)->pushCardBack(card);
			buffer->writeByte(CardsManager::getInstance()->getGameCardsID(card->getName()));
			buffer->writeByte(((GameCard *)card)->getNumber());
			buffer->writeByte(((GameCard *)card)->getPattern());
			
			card = _cardgroup->popCardFront();
			(*it)->pushCardBack(card);
			buffer->writeByte(CardsManager::getInstance()->getGameCardsID(card->getName()));
			buffer->writeByte(((GameCard *)card)->getNumber());
			buffer->writeByte(((GameCard *)card)->getPattern());

			buffer->setDataLenForPos();
			_cmdque.push(buffer);
			addRef(buffer);
		}
	}
	/*for(int i = 0; i < (int)_players.size()*2; i++)
	{
		Card *card;
		auto buffer = new CmdBuffer();
		buffer->setCmdCode(CMD_DRAWCARD);
		buffer->writeByte(2);

		card = _cardgroup->popCardFront();
		_players[i%_players.size()]->pushCardBack(card);
		buffer->writeByte(CardsManager::getInstance()->getGameCardsID(card->getName()));
		buffer->writeByte(((GameCard *)card)->getNumber());
		buffer->writeByte(((GameCard *)card)->getPattern());
		
		card = _cardgroup->popCardFront();
		_players[i%_players.size()]->pushCardBack(card);
		buffer->writeByte(CardsManager::getInstance()->getGameCardsID(card->getName()));
		buffer->writeByte(((GameCard *)card)->getNumber());
		buffer->writeByte(((GameCard *)card)->getPattern());

		buffer->setDataLenForPos();
		_cmdque.push(buffer);
		addRef(buffer);
	}*/

	//开始战斗
	_isstart = true;
	buffer = new CmdBuffer();
	buffer->setCmdCode(CMD_GAMESTART);
	buffer->setDataLenForPos();
	_cmdque.push(buffer);
	addRef(buffer);
	_handlestack.push(buffer);
	addRef(buffer);

	_curtime = TIME_START;
	_curplayer = _lifeplayers.begin();
	return _isstart = true;
}

void BattleFake::Process()
{
	if(_handlestack.size())
	{
		auto cmd = _handlestack.top();
		if(cmd->getCmdCode() == CMD_GAMESTART)
		{
			auto buffer = new CmdBuffer();
			buffer->setCmdCode(CMD_GAMETIME);
			buffer->writeDword(TIME_START);
			buffer->setDataLenForPos();
			_cmdque.push(buffer);
			addRef(buffer);
			_handlestack.push(buffer);
			addRef(buffer);
			return;
		}
		if(cmd->getCmdCode() == CMD_GAMETIME)
		{
			ulong time = cmd->readDword();
			PlayerFake *curplayer = (PlayerFake *)*_curplayer;
			auto buffer = new CmdBuffer();
			buffer->setCmdCode(CMD_SELECTCARD);
			buffer->writeByte(0);
			uchar count = 0;
			for(int i = 0; i < curplayer->getSize(); i++)
			{
				auto card = (GameCard *)curplayer->getCard(i);
				if(card->getUseTime() & time)
				{
					auto id = CardsManager::getInstance()->getGameCardsID(card->getCardIDName());
					buffer->writeByte(id);
					buffer->writeByte(card->getNumber());
					buffer->writeByte(card->getPattern());
					count++;
				}
			}
			if(count)
			{
				auto pos = buffer->getPos();
				buffer->resetPos();
				buffer->writeByte(count);
				buffer->setPos(pos);
				buffer->setDataLenForPos();
				_cmdque.push(buffer);
				addRef(buffer);
				_handlestack.push(buffer);
				addRef(buffer);
			}
			else
			{
				if(time == TIME_END)
					time = TIME_START;
				time = time << 1;
				buffer->setCmdCode(CMD_GAMETIME);
				buffer->resetPos();
				buffer->writeDword(time);
				buffer->setDataLenForPos();
				_cmdque.push(buffer);
				addRef(buffer);
				removeRef(cmd);
				_handlestack.pop();
				_handlestack.push(buffer);
				addRef(buffer);
			}
			return;
		}
	}
	return;
}