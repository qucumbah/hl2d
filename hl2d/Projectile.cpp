#include "Projectile.h"
#include "LineHitter.h"

Projectile::Projectile(
	double maxDistance,
	double speed,
	int startDamage,
	int endDamage,
	bool negatesPlayers,
	double addedAngle
) {
	_startDamage = startDamage;
	_endDamage = endDamage;
	_maxDistance = maxDistance;
	_speed = speed;
	_negatesPlayers = negatesPlayers;
	_addedAngle = addedAngle;
	_renderable = true;

	_distanceTravelled = 0;
	_id = getId();
}

void Projectile::activate(Player* player) {
	_type = "Projectile";
	_id = _nextId++;

	_x = player->getX();
	_y = player->getY();
	_angle = player->getAngle() + (_addedAngle / 180.0) * 3.14;
	_creatorId = player->getId();
}

void Projectile::update(
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

	if (hitsPlayers) {
		for (auto [player, _] : *playerHits) { //Dont need damage
			double fallof = _distanceTravelled / _maxDistance;
			double damage = util::lerp(_startDamage, _endDamage, fallof);

			player->hit((int)damage);
		}
	}

	if (hitsWall) {
		//Hits a wall on this tick
		//_explode(entities);
		destroy();
		return;
	}

	if (!_negatesPlayers && hitsPlayers) {
		//Stop if cant penetrate players
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

void Projectile::_move(Vec2 movement) {
	_x += movement.x;
	_y += movement.y;
}
