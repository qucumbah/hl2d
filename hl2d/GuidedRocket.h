#pragma once
#include "Rocket.h"

class GuidedRocket : public Rocket {
public:
	GuidedRocket(
		double maxDistance,
		double speed,
		int startDamage,
		int endDamage,
		bool negatesPlayers,
		double addedAngle,
		double blastRadius
	);

	virtual void activate(Player* player) override;

	virtual void update(
		Level* level,
		list<Entity*>* entities,
		string additionalInfo) override;

protected:
	virtual double _getUpdatedAngle(Level* level, list<Entity*>* entities);
};
