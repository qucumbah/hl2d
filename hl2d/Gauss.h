#pragma once

#include "Gun.h"
#include "LineHitter.h"

class Gauss : public Gun {
public:
	static const int COOLDOWN = 500;

	static const int RANGE = 1500;
	static const int MAX_ENERGY_LEVEL = 11;
	static const int PENETRATION_THRESHOLD = 5;
	static const int LOWEST_DAMAGE = 10;
	static const int HIGHEST_DAMAGE = 150;
	static const int DAMAGE_FALLOFF_PERCENT = 80;
	static const int TIME_TO_CHARGE = 100;

	Gauss(int ownerId);

	void pullTrigger1() override;
	void releaseTrigger1() override;
	void pullTrigger2() override;
	void releaseTrigger2() override;
protected:
	int _energyLevel;
	bool _isCharging;

	LineHitter* _getLaser(int chargeLevel);

	void _startCharging();
	void _stopCharging();

	void _charge();
};
