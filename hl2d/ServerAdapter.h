#pragma once

#include "WebsocketServer.h"
#include "Game.h"
#include "Timer.h"
#include <map>

using std::map;

class ServerAdapter {
public:
	ServerAdapter();
	~ServerAdapter();

	static const int TICKTIME = 10;
private:
	WebsocketServer _wss;
	Game _game;
	map<int, string>* _queuedInputs;
	Timer* _tickTimer;

	void _handleFirstRequest(int clientId, string message);
	void _handleRequest(int clientId, string message);

	void _tick();
};
