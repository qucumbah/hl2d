#include "Rocket.h"
#include "LineHitter.h"

Rocket::Rocket(
	double maxDistance,
	double speed,
	int startDamage,
	int endDamage,
	bool negatesPlayers,
	double addedAngle,
	double blastRadius
) : Projectile(
	maxDistance,
	speed,
	startDamage,
	endDamage,
	negatesPlayers,
	addedAngle
) {
	_blastRadius = blastRadius;
	_exploded = false;
}

string Rocket::getJson() {
	map<string, string> pairs;

	pairs["renderable"] = _renderable ? "true" : "false";
	pairs["type"] = _type;
	pairs["angle"] = to_string(_angle);
	pairs["radius"] = to_string(_radius);
	pairs["x"] = to_string(_x);
	pairs["y"] = to_string(_y);
	pairs["id"] = to_string(_id);

	pairs["exploded"] = _exploded ? "true" : "false";

	string json = util::createJson(pairs);
	return json;
}

void Rocket::activate(Player* player) {
	Projectile::activate(player);
	_type = "Rocket";
}

void Rocket::update(
	Level* level,
	list<Entity*>* entities,
	string additionalInfo
) {
	//Normalized direction vector
	Vec2 d = Vec2::getNormalFromAngle(_angle);

	double closestDistanceToWall = LineHitter::getClosestDistanceToWall(
		level, getPosition(), d, _speed
	);

	auto playerHits = LineHitter::getPlayerHits(
		level,
		entities,
		getPosition(),
		d,
		closestDistanceToWall,
		-1, //Damage is calculated differently for projectiles
		-1,
		_creatorId
	);

	bool hitsWall = closestDistanceToWall < _speed;
	bool hitsPlayers = playerHits->size() > 0;

	if (hitsWall || hitsPlayers) {
		_explode(entities);
		_exploded = true;
		destroy();
	}
	else {
		Vec2 movement = d * _speed;
		_move(movement);
		_distanceTravelled += movement.length();

		if (_distanceTravelled > _maxDistance) {
			destroy();
		}
	}
}

void Rocket::_explode(list<Entity*>* entities) {
	Vec2 explosion = getPosition();

	for (auto entity : *entities) {
		if (entity->getType() != "Player") {
			//Ignore other entities
			continue;
		}

		Player* player = (Player*)entity;

		Vec2 playerPosition = player->getPosition();
		double hitDistance = (playerPosition - explosion).length();
		if (hitDistance > _blastRadius) {
			//Player is too far from explosion
			continue;
		}

		double fallof = hitDistance / _blastRadius;
		double damage = util::lerp(_startDamage, _endDamage, fallof);

		player->hit((int)damage);
	}
}
