#include "LineHitter.h"
#include "util.h"

LineHitter::LineHitter(
	double range, int startDamage, int endDamage, bool negatesCover
) {
	_startDamage = startDamage;
	_endDamage = endDamage;
	_radius = range;
	_negatesCover = negatesCover;
}

void LineHitter::activate(Player* player) {
	_type = "AreaHitter";
	_id = _nextId++;

	_x = player->getX();
	_y = player->getY();
	_angle = player->getAngle();
	_radius = 200;
	_renderable = true;
	_creatorId = player->getId();
}

void LineHitter::update(
	Level* level,
	list<Entity*>* entities,
	string additionalInfo) {

	auto hits = _getPlayerHits(level, entities);
	for (auto [player, damage] : *hits) {
		player->hit( (int)damage );
	}
	delete hits;

	//Destroy after dealing damage
	destroy();
}

map<Player*, int>* LineHitter::_getPlayerHits(Level* level, list<Entity*>* entities) {
	//LineHitter hits the every player on a line with length RANGE that is
	//aimed at where creator is looking; after first update it gets destroyed

	auto hits = new map<Player*, int>();

	//Max hit distance (=hitscan line length)
	double maxDistance = _radius;
	//Normalized direction vector
	Vec2 d = Vec2::getNormalFromAngle(_angle);

	if (!_negatesCover) {
		maxDistance = _getClosestDistanceToWall(level, d, _radius);
	}

	for (auto entity : *entities) {
		if (entity->getType() != "Player") {
			//Ignore other entities
			continue;
		}

		Player* player = (Player*)entity;
		if (player->getId() == _creatorId) {
			//Dont hit creator
			continue;
		}

		Vec2 hit = Vec2::getIntersectionWithCircle(
			getPosition(), d, player->getPosition(), player->getRadius()
		);

		if (!hit.isValid()) {
			//Hit is invalid if there is no intersection
			continue;
		}

		double hitDistance = (hit - getPosition()).length();
		if (hitDistance > maxDistance) {
			//Player is behind a wall (if negatesCover == false) or is too far
			continue;
		}

		double fallof = hitDistance / maxDistance;
		double damage = util::lerp(_startDamage, _endDamage, fallof);

		(*hits)[player] = damage;
	}

	return hits;
}

double LineHitter::_getClosestDistanceToWall(
	Level* level,
	Vec2 direction,
	double maxDistance
) {
	//Distance to closest edge
	double k = maxDistance;

	for (auto edge : level->getEdges()) {
		double dist = Vec2::getIntersectionCoefficient(
			getPosition(), direction, edge.start, edge.body
		);

		if (dist >= 0 && dist < k) {
			k = dist;
		}
	}

	return k;
}