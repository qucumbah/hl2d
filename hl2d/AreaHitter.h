#pragma once

#include "Hitter.h"
#include "Player.h"

#include <string>

class AreaHitter : public Hitter {
public:
	static const int DAMAGE = 25;
	static const int RANGE = 50;
	virtual void activate(Player* player) override;

	virtual void update(
		Map* map,
		list<Entity*>* entities,
		string additionalInfo) override;
protected:
	bool _canHit(Player* player);
};