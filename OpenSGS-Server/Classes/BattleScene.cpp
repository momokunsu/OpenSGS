#include "Encoding.h"
#include "cocostudio/CocoStudio.h"
#include "CardsManager.h"
#include "BattleScene.h"
#include "UIPlayer.h"

using namespace cocos2d;

BattleScene *BattleScene::self = nullptr;

BattleScene::BattleScene(void)
{
}
BattleScene::~BattleScene(void)
{
	_battle->release();
	_sever->release();
	self = nullptr;
}

BattleScene* BattleScene::getInstance()
{
	if(!self)
	{
		self = create();
		self->retain();
	}
	return self;
}

void BattleScene::update(float dt)
{
	static bool isbusy = false;

	auto mainplayer = (UIPlayerMain*)getChildByTag(1);
	auto otherplayer = (UIPlayerOther*)getChildByTag(2);

	//服务器战斗处理
	if(!isbusy)
	{
		auto cmd = _battle->getResult();
		if(cmd)
		{
			auto tmpcmd = new CmdBuffer();
			auto k = cmd->getCmdCode();
			if(cmd->getCmdCode() == CMD_PLAYERINIT)
			{
				std::vector<Player *> playervec;
				_sever->getAllPlayers(playervec);
				for(auto it : playervec)
				{
					tmpcmd->setCmdCode(CMD_PLAYERINIT);
					tmpcmd->writeByte(it->getID());
					tmpcmd->writeByte(it->getPos());
					tmpcmd->writeText(it->getName());
					tmpcmd->writeByte(it->getMaxLifePoint());
					tmpcmd->writeByte(it->getLifePoint());
					tmpcmd->setDataLenForPos();
					_sever->broadcastCmd(tmpcmd);
				}
			}
		}
	}
	
	//服务端玩家指令处理
	if(mainplayer->getPlayer()->getCmdCount())
	{
	}

	if(otherplayer->getPlayer()->getCmdCount())
	{
	}

	return;
}

void BattleScene::reset()
{
	auto framesize = Director::getInstance()->getVisibleSize();
	_background->setScaleX(framesize.width/_background->getContentSize().width);
	_background->setScaleY(framesize.height/_background->getContentSize().height);
	_background->setPosition(getAnchorPointInPoints());

	getChildByTag(1)->setPositionX(getAnchorPointInPoints().x);
	getChildByTag(2)->setPosition(1040.0f, framesize.height - 15);
}

bool BattleScene::init()
{
	if(!Scene::init())
		return false;
	if(getChildrenCount() > 1)
	{
		reset();
		return true;
	}

	//加入背景图
	_background = Sprite::create("data/pics/background.jpg");
	auto framesize = Director::getInstance()->getVisibleSize();
	_background->setScaleX(framesize.width/_background->getContentSize().width);
	_background->setScaleY(framesize.height/_background->getContentSize().height);
	_background->setPosition(getAnchorPointInPoints());
	addChild(_background, -1);

	auto mainplayer = UIPlayerMain::create();
	mainplayer->setPositionX(getAnchorPointInPoints().x);
	mainplayer->setTag(1);
	addChild(mainplayer, 0);

	auto otherplayer = UIPlayerOther::create();
	otherplayer->setAnchorPoint(Vec2(0.5f, 1.0f));
	otherplayer->setPosition(1040.0f, framesize.height - 15);
	otherplayer->setTag(2);
	addChild(otherplayer, 1);
	
	//创建玩家
	auto player1 = new PlayerFake(NULL);
	player1->setName("MOMO");
	auto player2 = new PlayerFake(NULL);
	player2->setName("MaoZei");
	mainplayer->setPlayer(player1);
	otherplayer->setPlayer(player2);

	//创建服务器
	_sever = new GameServer();
	_sever->retain();
	_sever->addLocalPlayer(player1);
	_sever->addLocalPlayer(player2);

	//创建战斗处理器
	_battle = new BattleFake();
	_battle->retain();
	//加入玩家
	_battle->addPlayer(player1);
	_battle->addPlayer(player2);
	//加入卡组
	auto cm = CardsManager::getInstance();
	GamePackInfoVec vec;
	cm->getGamePack(vec);
	cm->addPackToCardGroup(vec[0].packName);
	auto cg = cm->createCardGroup();
	cg->shuffle();
	cg->shuffle();
	_battle->setCardgroup(cg);

	_battle->start();
	
	scheduleUpdate();

	//_sever->setHostPort(10086);
	//_sever->listen();

	return true;
}
