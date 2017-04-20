#include "GameServer.h"
#include "libiop/iop_server.h"

std::map<iop_base_t*, GameServer *> _severpool;

GameServer::GameServer(void)
{
	_hostaddress = "0.0.0.0";
	_hostport = NULL;
	_serverid = NULL;
	_islisten = false;
	_ccomcb = nullptr;
	_cntcb = nullptr;
	
	_iopbase = iop_base_new(5000);
	_severpool[_iopbase] = this;
	iop_thread_t th;
	iop_run_in_thread(_iopbase, &th);
}

GameServer::~GameServer(void)
{
	for(auto i : _playersmap)
	{
		removePlayer(i.second);
	}
	if(_iopbase)
	{
		iop_stop(_iopbase);
		iop_base_free(_iopbase);
		_iopbase = nullptr;
	}
}


bool GameServer::setHostAdress(const char *ip)
{
	if(_islisten)
		return false;
	_hostaddress = ip;
	return true;
}

bool GameServer::setHostPort(int port)
{
	if(_islisten)
		return false;
	_hostport = port;
	return true;
}

void GameServer::addLocalPlayer(Player *player)
{
	uchar i = 200;
	while(i > 0)
	{
		if(!_playersmap[i])
		{
			player->_id = i;
			player->setConnectState(true);
			_playersmap[i] = player;
			addRef(player);
			return;
		}
		i--;
	}
}

void GameServer::removePlayer(Player *player)
{
	auto sock = player->getNetHandle();
	/*if(sock)
		iop_del(_iopbase, player->_id);*/
	_playersmap.erase(player->_id);
	player->setConnectState(false);
	if(_cntcb)
		_cntcb(player, player->getID(), false);
	removeRef(player);
}

void GameServer::getAllPlayers(std::vector<Player*> &vec)
{
	if(!_islisten)
	{
		for(auto i : _playersmap)
		{
			vec.push_back(i.second);
		}
	}
}

void GameServer::getLocalAddress(std::vector<std::string> &ippool)
{
	auto hostinfo = gethostbyname(NULL);
	if(!hostinfo)
		return;

	int i = 0;
	while(hostinfo->h_addr_list[i])
	{
		ippool.push_back(inet_ntoa(*(in_addr *)hostinfo->h_addr_list[i]));
	}
}

int dataRecv(iop_base_t *base,int id,char *data,int len,void *arg)
{
	auto sever = _severpool[base];
	auto player = sever->_playersmap[id];
	while(len > 0)
	{
		auto cmd = new CmdBuffer();
		auto xlen = cmd->recvData(data, len);
		player->pushCmd(cmd);
		data = data+len-xlen;
		len = xlen;
		if(sever->_ccomcb)
			sever->_ccomcb(player, cmd);
	}
	if(!player->isConnect())
	{
		while(player->getCmdCount())
		{
			auto cmd = player->popCurCmd();
			if(cmd->getCmdCode() != CMD_PLAYERINIT)
				continue;
			else
			{
				player->setName(cmd->readText());
				player->setConnectState(true);
				if(sever->_cntcb)
					sever->_cntcb(player, id, true);
			}
		}
	}
	return 0;
}
void onConnect(iop_base_t *base, int id, void *arg)
{
	auto sever = _severpool[base];
	Player *player;
	if(sever->_playercstor)
		player = sever->_playercstor();
	else
		player = new Player(iop_get(base, id)->handle);

	iop_sockaddr_in addr;
	int len = sizeof(addr);
	getsockname(player->getNetHandle(), (iop_sockaddr *)&addr, &len);
	auto ip =inet_ntoa(addr.sin_addr);
	auto port = ntohs(addr.sin_port);

	sever->_playersmap[id] = player;
	sever->_setID(id);
	sever->addRef(player);
}
void onDestroy(iop_base_t *base, int id, void *arg)
{
	auto sever = _severpool[base];
	sever->removePlayer(sever->_playersmap[id]);
}
int onError(iop_base_t *base, int id,int err, void *arg)
{
	return -1;
}

void GameServer::listen(PlayerConnected cntevent, PlayerConstructor constor)
{
	if(!_islisten)
	{

		int sock = iop_udp_server("0.0.0.0", 0);
		bool optval=true;
		setsockopt(sock, SOL_SOCKET, SO_BROADCAST, (char*)&optval, sizeof(bool));

		iop_sockaddr_in addr;
		addr.sin_family=AF_INET;
		addr.sin_port=htons(12315);
		addr.sin_addr.s_addr = INADDR_BROADCAST;
		iop_sendto(sock, "fuck!", 6, 0, (iop_sockaddr *)&addr, sizeof(iop_sockaddr));

		_serverid = iop_add_tcp_server(_iopbase, _hostaddress.c_str(), _hostport, [](char *buf, int len)->int
		{
			return len;
		}, dataRecv, onConnect, onDestroy, onError, -1);

		_cntcb = cntevent;
		_playercstor = constor;
		_islisten = true;
	}
}

void GameServer::endListen()
{
	if(_islisten)
	{
		iop_del(_iopbase, _serverid);
		for(auto it = _playersmap.begin(); it != _playersmap.end();)
		{
			if(!(*it).second->isConnect())
			{
				auto player = (*it).second;
				it++;
				removePlayer(player);
			}
			else
				it++;
		}
		_islisten = false;
	}
}

void GameServer::sendCmd(Player *player, CmdBuffer *buffer)
{
	if(!player->getNetHandle())
	{
		auto tmpbuffer = new CmdBuffer();
		tmpbuffer->copyBuffer(buffer);
		player->pushCmd(tmpbuffer);
	}
	else
	{
		iop_buf_send(_iopbase, player->getID(), (const char*)buffer->getBufPtr(), buffer->getBufSize());
	}
}

void GameServer::broadcastCmd(CmdBuffer *buffer)
{
	for(auto it : _playersmap)
	{
		sendCmd(it.second, buffer);
	}
}