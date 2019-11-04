#pragma once
#include "Projectile.h"
class Rocket : public Projectile {
public:
	Rocket(
		double maxDistance,
		double speed,
		int startDamage,
		int endDamage,
		bool negatesPlayers,
		double addedAngle,
		double blastRadius
	);

	virtual string getJson() override;

	virtual void activate(Player* player) override;

	virtual void update(
		Level* level,
		list<Entity*>* entities,
		string additionalInfo) override;
protected:
	double _blastRadius;
	bool _exploded;

	void _explode(list<Entity*>* entities);
};

