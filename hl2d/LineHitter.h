#pragma once

#include "Hitter.h"
#include "Player.h"

#include <string>
#include <map>

using std::map;

class LineHitter : public Hitter {
public:
	LineHitter(
		double range,
		int startDamage,
		int endDamage,
		bool negatesCover,
		double addedAngle
	);

	virtual void activate(Player* player) override;

	virtual void update(
		Level* level,
		list<Entity*>* entities,
		string additionalInfo) override;

	static double getClosestDistanceToWall(
		Level* level,
		Vec2 position,
		Vec2 direction,
		double maxDistance
	);

	static map<Player*, int>* getPlayerHits(
		Level* level,
		list<Entity*>* entities,
		Vec2 position,
		Vec2 direction,
		double maxDistance,
		double startDamage,
		double endDamage,
		int creatorId
	);
protected:
	double _startDamage, _endDamage;
	bool _negatesCover;
	double _addedAngle;
};
