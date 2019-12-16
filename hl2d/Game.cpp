#include "Game.h"
#include "Player.h"
#include "util.h"

#include <iostream>

using std::list;
using std::vector;
using std::string;
using std::stringstream;
using std::cout;
using std::endl;
using std::exception;

Game::Game() {
	try {
		_level = new Level("a");
	}
	catch (exception& ex) {
		cout << ex.what() << endl;
	}
}

Game::~Game() {

}

void Game::addUnconfirmedPlayer(int playerId) {
	Player* player = new Player(playerId);
	spawnEntity(player);
	cout << "added player " << playerId <<
		"; entity number: " << _entities.size() << endl;
}

void Game::confirmPlayer(int playerId, string name) {
	bool nameTaken = isNameTaken(name);
	string newName = nameTaken ? (name + std::to_string(playerId)) : name;

	Player* player = _getPlayerById(playerId);
	if (player->getConnectionStatus() == Player::CONNECTED) {
		cout << "Client tried to connect twice; skipping last attempt" << endl;
		return;
	}
	player->connectWithName(newName);
}

void Game::removePlayer(int playerId) {
	Player* player = _getPlayerById(playerId);

	if (player == nullptr) {
		cout << "client " << playerId
			<< " sent disconnect request more than once; skipping" << endl;
		return;
	}

	player->destroy();
	cout << "removed player " << playerId <<
		"; entity number: " << _entities.size() << endl;
}

bool Game::isNameTaken(string name) {
	for (Entity* entity : _entities) {
		if (entity->getType() == "Player" &&
			((Player*)entity)->getName() == name
			) {
			return true;
		}
	}

	return false;
}

//Doesn't respawn players, just adds them to the entity list
void Game::spawnEntity(Entity* entity) {
	_entitiesBuffer.push_back(entity);
}
void Game::removeEntity(Entity* entity) {
	_entitiesBuffer.remove(entity);
}

void Game::respawnPlayer(Player* player) {
	vector<Level::Location> spawnLocations = _level->getSpawnLocations();
	if (spawnLocations.size() == 0) {
		player->respawn(0, 0);
		return;
	}

	int locationNumber = rand() % spawnLocations.size();
	Level::Location spawnLocation = spawnLocations[locationNumber];
	player->respawn(spawnLocation.x, spawnLocation.y);
}

string Game::getLevelJson() {
	return _level->getJson();
}

string Game::getJson() {
	stringstream result;

	int elementNumber = 1;
	int size = _entities.size();

	list<string> entities;
	for (Entity* entity : _entities) {
		entities.push_back( entity->getJson() );
	}

	return util::createJsonArray(entities);

	/*
	//Uncomment if you need some performance
	result << "[";
	for (Entity* entity : _entities) {
		result << entity->getJson();

		bool last = elementNumber == size;
		if (!last) {
			result << ",";
		}
		elementNumber++;
	}
	result << "]";

	return result.str();
	*/
}

void Game::update(map<int, string>* playerActions) {
	//We'll iterate over _entities but change the _entitiesBuffer. To know why
	//see header file.
	for (Entity* entity : _entities) {
		if (entity->isDestroyed()) {
			removeEntity(entity);
			delete entity;
			continue;
		}

		if (entity->getType() == "Player") {
			string thisPlayerActions = (*playerActions)[entity->getId()];

			Player* player = (Player*)entity;
			player->update(_level, &_entitiesBuffer, thisPlayerActions);
			if (!player->isAlive()) {
				EventQueue::after(3000, [this, player]() {
					respawnPlayer(player);
				});
			}
		}
		else {
			entity->update(_level, &_entitiesBuffer, "");
		}
	}

	_entities = _entitiesBuffer;
}

Player* Game::_getPlayerById(int playerId) {
	for (Entity* entity : _entitiesBuffer) {
		if (entity->getType() == "Player" && entity->getId() == playerId) {
			return (Player*)entity;
		}
	}

	return nullptr;
}