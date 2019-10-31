#pragma once

#include "Hitter.h"
#include "Player.h"

#include <string>

using std::string;

class AreaHitter : public Hitter {
public:
	AreaHitter(int damage, double range);

	virtual void activate(Player* player) override;

	virtual void update(
		Level* level,
		list<Entity*>* entities,
		string additionalInfo) override;
protected:
	bool _canHit(Player* player);
};