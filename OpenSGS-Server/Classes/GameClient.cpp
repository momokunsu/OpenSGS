#include "GameClient.h"


GameClient::GameClient(void)
{
	_isconnect = false;
	_severport = 0;
}
GameClient::~GameClient(void)
{
}

bool GameClient::setSeverAddress(const char *address)
{
	if(!_isconnect)
	{
		_severip = address;
		return true;
	}
	return false;
}

bool GameClient::setSeverPort(ushort port)
{
	if(!_isconnect)
	{
		_severport = port;
	}
	return false;
}

bool connectToSever(int waittime)
{
	return true;
}
