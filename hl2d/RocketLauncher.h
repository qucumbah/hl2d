#pragma once

#include "Gun.h"
#include "Rocket.h"

class RocketLauncher : public Gun {
public:
	static const int COOLDOWN = 500;
	static const int MAX_DISTANCE = 1000;
	static const int SPEED = 5;
	static const int START_DAMAGE = 200;
	static const int END_DAMAGE = 0;

	RocketLauncher(int ownerId);

	void pullTrigger1() override;
	void releaseTrigger1() override;
	void pullTrigger2() override;
	void releaseTrigger2() override;

	bool isInGuideMode();
protected:
	bool _isInGuideMode = true;
};
