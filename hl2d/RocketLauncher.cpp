#include "RocketLauncher.h"

RocketLauncher::RocketLauncher(int ownerId) : Gun(ownerId) {

}

void RocketLauncher::pullTrigger1() {
	_shoot(new Rocket(
		MAX_DISTANCE,
		SPEED,
		START_DAMAGE,
		END_DAMAGE,
		false,
		0,
		100
	), COOLDOWN);
}

void RocketLauncher::releaseTrigger1() {

}

void RocketLauncher::pullTrigger2() {
	_isInGuideMode = !_isInGuideMode;
}

void RocketLauncher::releaseTrigger2() {

}

bool RocketLauncher::isInGuideMode() {
	return _isInGuideMode;
}