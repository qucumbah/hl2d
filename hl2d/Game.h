#pragma once

#include "Entity.h"
#include "Map.h"
#include "Player.h"
#include <list>
#include <string>
#include <map>

using std::list;
using std::string;
using std::map;

class Game {
public:
	Game();
	~Game();

	void addUnconfirmedPlayer(int playerId);
	void confirmPlayer(int playerId, string name);
	void removePlayer(int playerId);

	bool isNameTaken(string name);

	void spawnEntity(Entity* entity);
	void removeEntity(Entity* entity);

	void respawnPlayer(Player* player);

	string getMapJson();
	string getJson();

	void update(map<int, string>* playerActions);
private:
	//We need two separate entitiy lists to avoid race condition. All add/remove
	//operations are performed on _entitiesBuffer, all read/iterate operations
	//use _entities. At the end of each update (=tick) we replace _entities with
	//_entitiesBuffer
	list<Entity*> _entities;
	list<Entity*> _entitiesBuffer;
	Map* _map;

	Player* _getPlayerById(int playerId);
};
