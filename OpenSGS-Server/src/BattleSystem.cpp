#include "BattleSystem.h"
#include "LogHandler.h"
#include <string>
#include <time.h>
#include <algorithm>



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
		LogHandler::setLog("BattleSystem::setPlayerLocal", ("invaild player local!!! index: " + std::to_string(pos)).c_str());
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
}

void BattleSystem::sufflePlayersLocal()
{
	std::random_shuffle(m_players.begin(), m_players.end());
	std::random_shuffle(m_players.begin(), m_players.end());
	return;
}

bool BattleSystem::startGame()
{
	if (m_players.size() < 4)
	{
		LogHandler::setLog("BattleSystem::startGame", "player min count is 4!!!");
		return false;
	}

	//分配玩家ID
	for (int i = 0; i < m_players.size(); i++)
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

	std::random_shuffle(m_statusgroup.begin(), m_statusgroup.end());
	std::random_shuffle(m_statusgroup.begin(), m_statusgroup.end());

	//游戏开始
	{
		LogHandler::setLog("BattleSystem::startGame", ("game start!!! player count is" + std::to_string(m_players.size())).c_str());
		auto ev = new EventGameStart();
		for (auto player : m_players)
		{
			player->dispatchEvent(ev);
		}
	}
	return true;
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

	//确定主公位置
	for (int i = 0; i < (int)m_statusgroup.size(); i++)
	{
		if (m_statusgroup[i] == ePlayerStatusType::Ruler)
		{
			m_cur_player = i;
			LogHandler::setLog("BattleSystem::distributeStatus", ("confirm the ruler location is pos: " + std::to_string(m_cur_player)).c_str());
			break;
		}
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

	//初始化流程
	m_cur_phrase = ePhraseType::Begin;
}

void BattleSystem::phraseStep()
{
	//流程开始
	//已死亡则跳过流程
	auto ev_phrase = new EventPhrase();
	ev_phrase->type = ePhraseType::Begin;
	ev_phrase->playerID = m_players[m_cur_player]->getID();
	while (true)
	{
		for (auto player : m_players)
		{
			player->dispatchEvent(ev_phrase);
		}

		ev_phrase->type = (ePhraseType)(((int)ev_phrase->type) + 1);
		for (auto player : m_players)
		{
			player->dispatchEvent(ev_phrase);
		}

		//指向下一阶段
		if (ev_phrase->type != ePhraseType::End)
			ev_phrase->type = (ePhraseType)(((int)ev_phrase->type) + 1);
		else
		{
			skipThisRound()
		}
	}
}

void BattleSystem::skipThisRound()
{
	m_cur_phrase = ePhraseType::Begin;
	m_cur_player = (m_cur_player + 1) % m_players.size();
}

void BattleSystem::handlePhrase(Player * player, ePhraseType ptype)
{
	switch (ptype)
	{
		case ePhraseType::Judge:
			break;
		case ePhraseType::Draw:
			char str[256] = { 0 };
			sprintf(str, "player %d get %d cards", (int)m_players[m_cur_player]->getID(), 2);
			LogHandler::setLog("BattleSystem::handlePhrase", str);
			break;
		case ePhraseType::Battle:
			break;
		case ePhraseType::Throw:
			break;
		default:
			break;
	}
}
