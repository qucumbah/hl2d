#include "Game.h"
#include "Player.h"

#include <iostream>

using std::list;
using std::string;
using std::stringstream;
using std::cout;
using std::endl;

Game::Game() {
	
}

Game::~Game() {

}

void Game::addUnconfirmedPlayer(int playerId) {
	Player* player = new Player(playerId);
	spawnEntity(player);
	//cout << "added player " << playerId <<
	//	"; entity number: " << _entities.size() << endl;
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
	player->destroy();
	//cout << "removed player " << playerId <<
	//	"; entity number: " << _entities.size() << endl;
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
	Map::Location spawnLocation = _map->getBestSpawnLocation();
	player->respawn(spawnLocation.x, spawnLocation.y);
}

string Game::getMapJson() {
	return _map->getJson();
}

string Game::getJson() {
	stringstream result;

	int elementNumber = 1;
	int size = _entities.size();

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
}

void Game::update(map<int, string>* playerActions) {
	//We'll iterate over _entities but change the _entitiesBuffer. To know why
	//see header file.
	for (Entity* entity : _entities) {
		if (entity->isDestroyed()) {
			removeEntity(entity);
		}

		if (entity->getType() == "Player") {
			string thisPlayerActions = (*playerActions)[entity->getId()];
			entity->update(_map, &_entitiesBuffer, thisPlayerActions);
		}
		else {
			entity->update(_map, &_entitiesBuffer, "");
		}
	}

	_entities = _entitiesBuffer;
}

Player* Game::_getPlayerById(int playerId) {
	for (Entity* entity : _entitiesBuffer) {
		if (entity->getType() == "Player") {
			Player* player = (Player*)entity;

			if (player->getId() == playerId) {
				return player;
			}
		}
	}

	return nullptr;
}