#include "Gauss.h"
#include "util.h"

Gauss::Gauss(int ownerId) : Gun(ownerId) {

}

void Gauss::pullTrigger1() {
	_shoot(_getLaser(1), COOLDOWN);
}

void Gauss::releaseTrigger1() {

}

void Gauss::pullTrigger2() {
	_startCharging();
}

void Gauss::releaseTrigger2() {
	_stopCharging();
}

LineHitter* Gauss::_getLaser(int energyLevel) {
	int startDamage = util::lerp(
		LOWEST_DAMAGE,
		HIGHEST_DAMAGE,
		1.0 * energyLevel / MAX_ENERGY_LEVEL
	);
	int endDamage = startDamage * DAMAGE_FALLOFF_PERCENT / 100;

	bool penetratesWalls = energyLevel >= PENETRATION_THRESHOLD;
	return new LineHitter(RANGE, startDamage, endDamage, penetratesWalls, 0);
}

void Gauss::_startCharging() {
	_isCharging = true;
	_charge();
}

void Gauss::_stopCharging() {
	_isCharging = false;
	_shoot(_getLaser(_energyLevel), COOLDOWN);
	_energyLevel = 1;
}

void Gauss::_charge() {
	if (_isOnCooldown || !_isCharging) {
		return;
	}

	_energyLevel++;

	if (_energyLevel == MAX_ENERGY_LEVEL) {
		return;
	}

	EventQueue::after(TIME_TO_CHARGE, [this]() { _charge(); });
}