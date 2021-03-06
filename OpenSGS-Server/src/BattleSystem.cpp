﻿#include "BattleSystem.h"
#include "LogHandler.h"
#include "CardsManager.h"
#include "libs/StringManager.h"

#include <time.h>
#include <string>
#include <algorithm>

typedef utli::StringManager STR;

using namespace std;

BattleSystem *BattleSystem::self = nullptr;

BattleSystem::BattleSystem()
{
	//初始化随机数生成器
	srand((uint)time(nullptr));
	//初始化大小端转化器
	//GameEvent::initEndian();
}

BattleSystem::~BattleSystem()
{
}

BattleSystem* BattleSystem::getInstance()
{
	if (!self)
		self = new BattleSystem();

	return self;
}

bool BattleSystem::addPlayer(Player * player)
{
	if (m_players.size() > 10)
	{
		LogWarn("BattleSystem::addPlayer", "player max count is 10!!!");
		return false;
	}

	m_players.push_back(player);
	return true;
}

bool BattleSystem::setPlayerLocation(Player *player, int pos)
{
	if (pos < 0 || pos > (int)m_players.size() - 1)
	{
		LogWarn("BattleSystem::setPlayerLocation", STR::format("invaild player location!!! index: %d", pos));
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
			LogInfo("BattleSystem::setPlayerLocation", STR::format("set player location to index: %d", pos));
			return true;
		}
	}
	LogWarn("BattleSystem::setPlayerLocation", "player not found!!");
	return false;
}

void BattleSystem::sufflePlayersLocation()
{
	vector<Player *> tmp_vec = m_players;
	SuffleVector(m_players);
	string log = "suffle the players:\n";
	for (size_t i = 0; i < tmp_vec.size(); i++)
	{
		for (size_t j = 0; j < m_players.size(); j++)
		{
			if (tmp_vec[i] == m_players[j])
			{
				log += STR::format("player[%d] ---> player[%d]\n", i + 1, j + 1);
			}
		}
	}
	LogInfo("BattleSystem::sufflePlayersLocation", log.c_str());
	return;
}

void BattleSystem::shuffleCardsDeck()
{
	SuffleList(m_card_deck);
}

bool BattleSystem::initGame()
{
	if (m_players.size() < 4)
	{
		LogError("BattleSystem::initGame", "player min count is 4!!!");
		return false;
	}

	//分配玩家ID 与 位置
	for (int i = 0; i < (int)m_players.size(); i++)
	{
		m_players[i]->setID(i + 1);
		m_players[i]->setLocation(i);
		m_id_players[m_players[i]->getID()] = m_players[i];
	}

	//根据人数匹配身份
	m_statusgroup.push_back(ePlayerStatusType::Ruler);
	m_statusgroup.push_back(ePlayerStatusType::Courtier);
	m_statusgroup.push_back(ePlayerStatusType::Spy);
	m_statusgroup.push_back(ePlayerStatusType::Rebel);
	if (m_players.size() > 4)
		m_statusgroup.push_back(ePlayerStatusType::Rebel);
	if (m_players.size() > 5)
	{
		m_statusgroup.erase(std::find(m_statusgroup.begin(), m_statusgroup.end(), ePlayerStatusType::Spy));
		m_statusgroup.push_back(ePlayerStatusType::Courtier);
		m_statusgroup.push_back(ePlayerStatusType::Rebel);
	}
	if (m_players.size() > 6)
		m_statusgroup.push_back(ePlayerStatusType::Spy);
	if (m_players.size() > 7)
		m_statusgroup.push_back(ePlayerStatusType::Rebel);
	if (m_players.size() > 8)
		m_statusgroup.push_back(ePlayerStatusType::Courtier);
	if (m_players.size() > 9)
		m_statusgroup.push_back(ePlayerStatusType::Rebel);
	SuffleVector(m_statusgroup);

	m_cur_phrase = ePhraseType::Begin;
	m_drawcount = m_global_drawcount = 2;
	m_card_recycle_bin.clear();

	return true;
}

void BattleSystem::startGame()
{
	//游戏开始
	EventGameStart ev;
	ev.PlayerGroup = &m_players;
	broadcastEvent(ev);
	LogInfo("BattleSystem::startGame", STR::format("game start!!! player count is %d", m_players.size()));
}

void BattleSystem::dealStatus()
{
	//分发身份
	EventGetPlayerStatus ev;
	for (int i = 0; i < (int)m_players.size(); i++)
	{
		ev.PlayerStatusMap[m_players[i]->getID()] = m_statusgroup[i];
	}
	broadcastEvent(ev);
	for (auto it : ev.PlayerStatusMap)
	{
		m_id_players[it.first]->setPlayerStatus(it.second);
	}
	LogInfo("BattleSystem::dealStatus", "dispatch player status");
}

void BattleSystem::dealCards()
{
	for (int i = 0; i < 2; i++)
	{
		for (auto player : m_players)
		{
			drawCards(player->getID(), 2);
		}
	}
}

void BattleSystem::startBattle()
{
	//确定主公位置
	for (auto player : m_players)
	{
		if (player->getPlayerStatus() == ePlayerStatusType::Ruler)
		{
			m_cur_player = player->getLocation();
			LogInfo("BattleSystem::initGame", STR::format("confirm the ruler location is pos: %d", m_cur_player));
			break;
		}
	}

	//战斗开始
	EventBattleStart ev;
	ev.StartPlayerId = m_players[m_cur_player]->getID();
	ev.StartPhrase = m_cur_phrase;
	broadcastEvent(ev);
	m_cur_player = m_id_players[ev.StartPlayerId]->getLocation();
	m_cur_phrase = ev.StartPhrase;
	LogInfo("BattleSystem::startBattle", "battle start!!!");
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
	EventPhrase ev_phrase;
	ev_phrase.PhraseType = m_cur_phrase;
	ev_phrase.TargetId = m_players[m_cur_player]->getID();
	broadcastEvent(ev_phrase);
	m_cur_phrase = ev_phrase.PhraseType;
	m_cur_player = m_id_players[ev_phrase.TargetId]->getLocation();

	//执行流程
	handlePhrase(m_players[m_cur_player], m_cur_phrase);

	//指向下一阶段
	if (ev_phrase.PhraseType != ePhraseType::End)
		ev_phrase.PhraseType = (ePhraseType)(((int)ev_phrase.PhraseType) + 1);
	else
	{
		skipThisTurn();
	}
}

void BattleSystem::drawCards(uchar playerId, int count, int index)
{
	auto player = m_id_players[playerId];
	if (!player)
	{
		LogError("BattleSystem::drawCards", STR::format("player %d not exist!!", playerId));
		return;
	}

	auto ev = EventGetCards();
	std::string str = "";
	ev.GetType = eGetCardType::Draw;
	ev.TargetId = playerId;
	for (int i = 0; i < count; i++)
	{
		ev.Cards.push_back(drawCard());
    str += STR::format("%d ", (int)ev.Cards.back());
	}

	LogInfo("BattleSystem::drawCards", STR::format("player %d draw %d cards:\n%s", playerId, m_drawcount, str.c_str()));
	broadcastEvent(ev);
}

void BattleSystem::useCard(uchar userId, uint cardPos, uchar targetId)
{
	auto user = m_id_players[userId];
	if (!user)
	{
		LogError("BattleSystem::useCard", STR::format("userid %d not exist!!", userId));
		return;
	}
	auto cards = user->getHandCards();
	if (cardPos >= cards.size())
	{
		LogError("BattleSystem::useCard", STR::format("invalid card pos: %d, max size is %d!!", cardPos, (int)cards.size()));
		return;
	}

  cards.erase(cards.begin() + cardPos);
  useCard(userId, CardsManager::getCardInfo(cards[cardPos]), targetId);
}

void BattleSystem::useCard(uchar userId, Card* card, uchar targetId, bool canThrow)
{
  auto user = m_id_players[userId];
  if (!user)
  {
    LogError("BattleSystem::useCard", STR::format("userid %d not exist!!", userId));
    return;
  }
  
  auto ev = EventUseCard();
  ev.UserId = userId;
  ev.TargetId = targetId;
  ev.UseCard = card;
  ev.CanThrow = canThrow;
  m_used_card_list.push_back(ev);
  broadcastEvent(m_used_card_list.back());
}

void BattleSystem::throwCard(const std::vector<uint>& cards, eThrowCardType throwType)
{
  auto ev = EventThrowCards();
  ev.Cards = cards;
  ev.ThrowType = throwType;
  broadcastEvent(ev);
  for (auto card : ev.Cards)
  {
    m_card_recycle_bin.push_back(card);
  }
}

void BattleSystem::handleUseCards()
{
  while (m_used_card_list.size() > 0)
  {
    // 弃牌
    std::vector<uint> vec(1);
    vec.push_back(m_used_card_list.back().UseCard->getId());
    throwCard(vec);
  }
}

void BattleSystem::skipThisTurn()
{
	m_cur_phrase = ePhraseType::Begin;
	m_cur_player = (m_cur_player + 1) % m_players.size();
	m_drawcount = m_global_drawcount;
}

void BattleSystem::broadcastEvent(GameEvent& ev)
{
	for (auto player : m_players)
	{
		player->dispatchEvent(ev);
	}
}

void BattleSystem::handlePhrase(Player * player, ePhraseType ptype)
{
	switch (ptype)
	{
		case ePhraseType::Judge:
			break;
		case ePhraseType::Draw:
			drawCards(player->getID(), m_drawcount);
			break;
		case ePhraseType::Battle:
			break;
		case ePhraseType::Throw:
			break;
		default:
			break;
	}
}

uint BattleSystem::drawCard()
{
	if (m_card_deck.size() < 1)
	{
		m_card_recycle_bin.merge(m_card_deck);
		SuffleList(m_card_deck);
	}

	auto ret = m_card_deck.front();
	m_card_deck.pop_front();
	return ret;
}
