#include "BattleSystem.h"
#include "LogHandler.h"
#include <string>
#include <time.h>
#include <algorithm>



BattleSystem::BattleSystem()
{
	//初始化随机数生成器
	srand(time(nullptr));
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
	if(pos < 0 || pos > (int)m_players.size() - 1)
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
	if(m_players.size() < 4)
	{
		LogHandler::setLog("BattleSystem::startGame", "player min count is 4!!!");
		return false;
	}

	//根据人数匹配身份
	m_statusgroup.push_back(ePlayerStatusType::Ruler);
	m_statusgroup.push_back(ePlayerStatusType::Subject);
	m_statusgroup.push_back(ePlayerStatusType::Spy);
	m_statusgroup.push_back(ePlayerStatusType::Rebel);
	if(m_players.size() > 4)
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
	LogHandler::setLog("BattleSystem::startGame", ("game start!!! player count is" + std::to_string(m_players.size())).c_str());
	for (auto player : m_players)
	{
		player->dispatchEvent(ePlayerEvent::GameStart, nullptr);
	}

	//分发身份
	LogHandler::setLog("BattleSystem::startGame", "dispatch player status");
	for (int i = 0; i < (int)m_players.size(); i++)
	{
		m_players[i]->dispatchEvent(ePlayerEvent::GetPlayerStatus, &m_statusgroup[i]);
	}

	//确定主公位置
	LogHandler::setLog("BattleSystem::startGame", ("confirm the ruler location is pos: " + std::to_string(m_players.size())).c_str());
	for (int i = 0; i < (int)m_statusgroup.size(); i++)
	{
		if (m_statusgroup[i] == ePlayerStatusType::Ruler)
		{
			m_cur_player = i;
			break;
		}
	}

	//

	return true;
}
