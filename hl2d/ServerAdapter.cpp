#include "ServerAdapter.h"
#include "EventQueue.h"

using std::map;
using std::stringstream;

ServerAdapter::ServerAdapter() {
	_queuedInputs = new map<int, string>();

	_tickTimerId = EventQueue::every(TICKTIME, [this]() { this->_tick(); });

	_wss.on("clientConnect", [this](int clientId, string message) {
		this->_game.addUnconfirmedPlayer(clientId);
	});

	_wss.on("clientDisconnect", [this](int clientId, string message) {
		this->_game.removePlayer(clientId);
	});

	_wss.on("clientMessage", [this](int clientId, string message) {
		if (message.find("getMapInfo") != -1) {
			this->_handleFirstRequest(clientId, message);
		}
		else if (message.find("inputs") != -1) {
			this->_handleRequest(clientId, message);
		}
	});
}

ServerAdapter::~ServerAdapter() {
	EventQueue::cancel(_tickTimerId);
	while (_isExecutingGameTick) {
		Sleep(10);
	}
}

void ServerAdapter::_handleFirstRequest(int clientId, string message) {
	//Request contains 'getMapInfo', which means that the player has
	//just connected and is now trying to get map info;
	//At the same time he sends the desired name, which is mirrored if
	//it's free or gets returned with player's ID appended to it if
	//it's already taken;
	//Response to client's first message consists of his new name and
	//map JSON
	int nameStart = message.find("name ") + 5;
	int nameEnd = message.find("\n", nameStart);
	string name = message.substr(nameStart, nameEnd - nameStart);

	this->_game.confirmPlayer(clientId, name);

	string levelJson = this->_game.getLevelJson();
	string response = "id " + std::to_string(clientId) + "\n" + levelJson;
	this->_wss.sendMessage(response, clientId);
}

void ServerAdapter::_handleRequest(int clientId, string message) {
	//Every tick we call Game::update() with all of the recorded player's
	//actions. But player can send more than one set of actions between ticks,
	//so we have to add all of them to queue
	string previousInputs = (*_queuedInputs)[clientId];
	//Input request starts with line "intputs\n" which we dont need
	string newInputs = message.substr( message.find("\n") + 1 );

	(*_queuedInputs)[clientId] = previousInputs + newInputs;
}

void ServerAdapter::_tick() {
	_isExecutingGameTick = true;

	string gameState = this->_game.getJson();
	this->_wss.sendMessageAll(gameState);

	//This little bit of code caused around 10% of all packages to disappear
	//This is how it looked before:
	//
	//this->_game.update(_queuedInputs);
	//_queuedInputs = new map<int, string>();
	//
	//While the game was updating clients would still send messages, but these
	//messages would get skipped because queued inputs would be cleaned after
	//update.
	//After this simple fix there are much less lost packages
	map<int, string>* inputs = _queuedInputs;
	_queuedInputs = new map<int, string>();
	this->_game.update(inputs);
	delete inputs;

	_isExecutingGameTick = false;
}