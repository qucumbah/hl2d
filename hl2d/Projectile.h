#pragma once

#include "Hitter.h"
#include "Player.h"

class Projectile : public Hitter {
public:
	Projectile(
		double maxDistance,
		double speed,
		int startDamage,
		int endDamage,
		bool negatesPlayers,
		double addedAngle
	);

	virtual void activate(Player* player) override;

	virtual void update(
		Level* level,
		list<Entity*>* entities,
		string additionalInfo) override;
protected:
	double _startDamage, _endDamage;
	bool _negatesPlayers;
	double _addedAngle;
	double _speed;

	double _distanceTravelled;

	void _explode(list<Entity*>* entities);
	void _move(Vec2 movement);
};
