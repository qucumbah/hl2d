#pragma once

#include "Gun.h"
#include "LineHitter.h"

class Rifle : public Gun {
public:
	static const int COOLDOWN = 100;
	static const int INACCURACY = 5;

	Rifle(int ownerId);

	void pullTrigger1() override;
	void releaseTrigger1() override;
	void pullTrigger2() override;
	void releaseTrigger2() override;

protected:
	bool _isShooting = false;

	LineHitter* _createBullet();
	void _startShooting();
	void _stopShooting();
	virtual void _shoot(Hitter* hitter, int cooldown) override;
};
