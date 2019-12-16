#include "GuidedRocket.h"
#include "LineHitter.h"
#include "LaserPointer.h"

GuidedRocket::GuidedRocket(
	double maxDistance,
	double speed,
	int startDamage,
	int endDamage,
	bool negatesPlayers,
	double addedAngle,
	double blastRadius
) : Rocket(
	maxDistance,
	speed,
	startDamage,
	endDamage,
	negatesPlayers,
	addedAngle,
	blastRadius
) {

}

void GuidedRocket::activate(Player* player) {
	Rocket::activate(player);
	_type = "GuidedRocket";
}

void GuidedRocket::update(
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
		_angle = _getUpdatedAngle(level, entities);

		Vec2 movement = d * _speed;
		_move(movement);
		_distanceTravelled += movement.length();

		if (_distanceTravelled > _maxDistance) {
			destroy();
		}
	}
}

double GuidedRocket::_getUpdatedAngle(Level* level, list<Entity*>* entities) {
	double closestDistanceToPointer = INFINITY;
	LaserPointer* closestPointer = nullptr;

	for (auto* entity : *entities) {
		if (entity->getType() != "LaserPointer") {
			//We only need laser pointers
			continue;
		}

		LaserPointer* pointer = (LaserPointer*)entity;
		Vec2 pointerPosition = pointer->getPosition();
		Vec2 thisPosition = getPosition();

		Vec2 direction = (pointerPosition - thisPosition).normal();
		double distanceToPointer = (pointerPosition - thisPosition).length();

		double distanceToWall = LineHitter::getClosestDistanceToWall(
			level,
			thisPosition,
			direction,
			distanceToPointer
		);
		
		if (distanceToWall < distanceToPointer) {
			//There was a wall blocking rocket's line of sight
			continue;
		}

		if (distanceToPointer < closestDistanceToPointer) {
			closestDistanceToPointer = distanceToPointer;
			closestPointer = pointer;
		}
	}

	if (!_finite(closestDistanceToPointer)) {
		//Couldn't find unblocked laser pointer; keep direction
		return _angle;
	}

	Vec2 currentDirection = Vec2::getNormalFromAngle(_angle);
	Vec2 desiredDirection = (closestPointer->getPosition() - this->getPosition()).normal();
	double angle = Vec2::getAngleBetween(currentDirection, desiredDirection);

	const double MAX_ANGLE_CHANGE_PER_TICK = 0.02;
	double angleChange;
	if (abs(angle) < MAX_ANGLE_CHANGE_PER_TICK) {
		angleChange = angle;
	}
	else {
		angleChange = ((angle < 0) ? -1 : 1) * MAX_ANGLE_CHANGE_PER_TICK;
	}

	return _angle + angleChange;
}
