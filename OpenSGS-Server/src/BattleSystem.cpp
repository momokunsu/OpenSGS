#include "BattleSystem.h"
#include "LogHandler.h"
#include "libs\StringManager.h"

#include <string>
#include <time.h>
#include <algorithm>

typedef StringManager STR;


BattleSystem::BattleSystem()
{
	//初始化随机数生成器
	srand(time(nullptr));
	//初始化大小端转化器
	GameEvent::initEndian();
}


BattleSystem::~BattleSystem()
{
}

bool BattleSystem::addPlayer(Player * player)
{
	if (m_players.size() > 10)
	{
		LogHandler::setLog("BattleSystem::addPlayer", "player max count is 10!!!");
		return false;
	}

	m_players.push_back(player);
	return true;
}

bool BattleSystem::setPlayerLocal(Player *player, int pos)
{
	if (pos < 0 || pos >(int)m_players.size() - 1)
	{
		LogHandler::setLog("BattleSystem::setPlayerLocal", STR::format("invaild player local!!! index: %d", pos));
		return false;
	}

	//交换
	for (int i = 0; i < (int)m_players.size(); i++)
	{
		if (m_players[i] == player)
		{
			Player *n = m_players[i];
			m_players[i] = m_players[pos];
			m_players[pos] = n;
		}
	}
	return true;
}

void BattleSystem::sufflePlayersLocal()
{
	SuffleVector(m_players);
	return;
}

bool BattleSystem::initGame()
{
	if (m_players.size() < 4)
	{
		LogHandler::setLog("BattleSystem::initBattleInfo", "player min count is 4!!!");
		return false;
	}

	//分配玩家ID
	for (int i = 0; i < (int)m_players.size(); i++)
	{
		m_players[i]->setID(i + 1);
	}

	//根据人数匹配身份
	m_statusgroup.push_back(ePlayerStatusType::Ruler);
	m_statusgroup.push_back(ePlayerStatusType::Subject);
	m_statusgroup.push_back(ePlayerStatusType::Spy);
	m_statusgroup.push_back(ePlayerStatusType::Rebel);
	if (m_players.size() > 4)
		m_statusgroup.push_back(ePlayerStatusType::Rebel);
	if (m_players.size() > 5)
	{
		m_statusgroup.erase(std::find(m_statusgroup.begin(), m_statusgroup.end(), ePlayerStatusType::Spy));
		m_statusgroup.push_back(ePlayerStatusType::Subject);
		m_statusgroup.push_back(ePlayerStatusType::Rebel);
	}
	if (m_players.size() > 6)
		m_statusgroup.push_back(ePlayerStatusType::Spy);
	if (m_players.size() > 7)
		m_statusgroup.push_back(ePlayerStatusType::Rebel);
	if (m_players.size() > 8)
		m_statusgroup.push_back(ePlayerStatusType::Subject);
	if (m_players.size() > 9)
		m_statusgroup.push_back(ePlayerStatusType::Rebel);
	SuffleVector(m_statusgroup);


	//确定主公位置
	for (int i = 0; i < (int)m_statusgroup.size(); i++)
	{
		if (m_statusgroup[i] == ePlayerStatusType::Ruler)
		{
			m_cur_player = i;
			LogHandler::setLog("BattleSystem::initBattleInfo", STR::format("confirm the ruler location is pos: %d", m_cur_player));
			break;
		}
	}

	m_cur_phrase = ePhraseType::Begin;
	m_drawcount = m_global_drawcount = 2;
	m_card_recycle_bin.clear();

	return true;
}

void BattleSystem::startGame()
{
	//游戏开始
	LogHandler::setLog("BattleSystem::startGame", STR::format("game start!!! player count is %d", m_players.size()));
	auto ev = new EventGameStart();
	for (auto player : m_players)
	{
		player->dispatchEvent(ev);
	}
}

void BattleSystem::distributeStatus()
{
	//分发身份
	LogHandler::setLog("BattleSystem::distributeStatus", "dispatch player status");
	auto ev = new EventGetPlayerStatus();
	for (int i = 0; i < (int)m_players.size(); i++)
	{
		ev->statusMap[m_players[i]->getID()] = m_statusgroup[i];
	}
	for (auto player : m_players)
	{
		player->dispatchEvent(ev);
	}
}

void BattleSystem::startBattle()
{
	//战斗开始
	LogHandler::setLog("BattleSystem::startBattle", "battle start!!!");
	auto ev = new EventBattleStart();
	for (auto player : m_players)
	{
		player->dispatchEvent(ev);
	}
}

void BattleSystem::phraseStep()
{
	//已死亡则跳过流程
	if (m_players[m_cur_player]->isDead())
	{
		skipThisTurn();
		return;
	}

	//流程开始
	auto ev_phrase = new EventPhrase();
	ev_phrase->type = m_cur_phrase;
	ev_phrase->playerID = m_players[m_cur_player]->getID();
	while (true)
	{
		//处理事件
		for (auto player : m_players)
		{
			player->dispatchEvent(ev_phrase);
		}

		//执行流程
		handlePhrase(m_players[m_cur_player], m_cur_phrase);

		//指向下一阶段
		if (ev_phrase->type != ePhraseType::End)
			ev_phrase->type = (ePhraseType)(((int)ev_phrase->type) + 1);
		else
		{
			skipThisTurn();
		}
	}
}

void BattleSystem::skipThisTurn()
{
	m_cur_phrase = ePhraseType::Begin;
	m_cur_player = (m_cur_player + 1) % m_players.size();
	m_drawcount = m_global_drawcount;
}

void BattleSystem::handlePhrase(Player * player, ePhraseType ptype)
{
	switch (ptype)
	{
		case ePhraseType::Judge:
			break;
		case ePhraseType::Draw:
			LogHandler::setLog("BattleSystem::handlePhrase", STR::format("player %d get %d cards", (int)player->getID(), m_drawcount));
			break;
		case ePhraseType::Battle:
			break;
		case ePhraseType::Throw:
			break;
		default:
			break;
	}
}
