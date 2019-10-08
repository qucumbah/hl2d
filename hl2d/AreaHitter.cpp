#include "AreaHitter.h"

void AreaHitter::activate(Player* player) {
	_type = "AreaHitter";
	_id = _nextId++;
	
	_x = player->getX();
	_y = player->getY();
	_angle = player->getAngle();
	_renderable = true;
	_creatorId = player->getId();
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
	//AreaHitters hits everyone except it's creator in semicircle aimed at the
	//creator's view direction
	if (player->getId() == _creatorId) {
		return false;
	}

	Vec2 relativePosition = player->getPosition() - this->getPosition();

	if (relativePosition.length() > RANGE) {
		//Player is too far
		return false;
	}

	Vec2 hitDirectionNormal = getDirectionNormal();

	if (relativePosition * hitDirectionNormal < 0) {
		//Player is behind
		return false;
	}

	return true;
}