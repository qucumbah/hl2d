#include "LineHitter.h"
#include "util.h"

LineHitter::LineHitter(
	double range,
	int startDamage,
	int endDamage,
	bool negatesCover,
	double addedAngle
) {
	_startDamage = startDamage;
	_endDamage = endDamage;
	_maxDistance = range;
	_negatesCover = negatesCover;
	_addedAngle = addedAngle;
	_renderable = false;
}

void LineHitter::activate(Player* player) {
	_type = "LineHitter";
	_id = _nextId++;

	_x = player->getX();
	_y = player->getY();
	_angle = player->getAngle() + (_addedAngle / 180.0) * 3.14;
	_creatorId = player->getId();
}

void LineHitter::update(
	Level* level,
	list<Entity*>* entities,
	string additionalInfo
) {
	//Max hit distance (=hitscan line length)
	double maxDistance = _maxDistance;
	//Normalized direction vector
	Vec2 d = Vec2::getNormalFromAngle(_angle);

	if (!_negatesCover) {
		maxDistance = LineHitter::getClosestDistanceToWall(
			level, getPosition(), d, _maxDistance);
	}

	//Get all hits on hit line
	auto hits = LineHitter::getPlayerHits(
		level,
		entities,
		getPosition(),
		d,
		maxDistance,
		_startDamage,
		_endDamage,
		_creatorId
	);

	for (auto [player, damage] : *hits) {
		player->hit( damage );
	}
	delete hits;

	//Change radius to hit line length for presentation (we dont need it for
	//anything else)
	_radius = maxDistance;
	_renderable = true;

	//Destroy after dealing damage
	destroy();
}

double LineHitter::getClosestDistanceToWall(
	Level* level,
	Vec2 position,
	Vec2 direction,
	double maxDistance
) {
	//Distance to closest edge
	double k = maxDistance;

	for (auto edge : level->getEdges()) {
		double dist = Vec2::getIntersectionCoefficient(
			position, direction, edge.start, edge.body
		);

		if (dist >= 0 && dist < k) {
			k = dist;
		}
	}

	return k;
}

//LineHitter hits the every player on a line with length RANGE that is
//aimed at where creator is looking; after first update it gets destroyed
map<Player*, int>* LineHitter::getPlayerHits(
	Level* level,
	list<Entity*>* entities,
	Vec2 position,
	Vec2 direction,
	double maxDistance,
	double startDamage,
	double endDamage,
	int creatorId
) {
	auto hits = new map<Player*, int>();

	for (auto entity : *entities) {
		if (entity->getType() != "Player") {
			//Ignore other entities
			continue;
		}

		Player* player = (Player*)entity;
		if (player->getId() == creatorId) {
			//Dont hit creator
			continue;
		}

		Vec2 hit = Vec2::getIntersectionWithCircle(
			position, direction, player->getPosition(), player->getRadius()
		);

		if (!hit.isValid()) {
			//Hit is invalid if there is no intersection
			continue;
		}

		double hitDistance = (hit - position).length();
		if (hitDistance > maxDistance) {
			//Player is behind a wall (if negatesCover == false) or is too far
			continue;
		}

		double fallof = hitDistance / maxDistance;
		double damage = util::lerp(startDamage, endDamage, fallof);

		(*hits)[player] = (int)damage;
	}

	return hits;
}
