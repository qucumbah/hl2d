#include "AreaHitter.h"

void AreaHitter::activate(
	double x,
	double y,
	double angle,
	int creatorId
) {
	_type = "AreaHitter";
	_id = _nextId++;

	_x = x;
	_y = y;
	_angle = angle;
	_renderable = true;
	_creatorId = creatorId;
}

void AreaHitter::update(
	Map* map,
	list<Entity*>* entities,
	string additionalInfo) {

	//Hit on creation/first update
	for (auto entity : *entities) {
		if (entity->getType() == "Player") {
			Player* player = (Player*)entity;
			if (_canHit(player)) {
				player->hit(DAMAGE);
			}
		}
	}

	//Destroy after dealing damage
	destroy();
}

bool AreaHitter::_canHit(Player* player) {
	cout << _creatorId << player->getId() << endl;
	return player->getId() != _creatorId;
}