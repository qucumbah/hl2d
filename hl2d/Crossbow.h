#pragma once

#include "Gun.h"
#include "Projectile.h"

class Crossbow : public Gun {
public:
	static const int COOLDOWN = 500;
	static const int MAX_DISTANCE = 1000;
	static const int SPEED = 8;
	static const int DAMAGE = 100;

	Crossbow(int ownerId);

	void pullTrigger1() override;
	void releaseTrigger1() override;
	void pullTrigger2() override;
	void releaseTrigger2() override;
};
