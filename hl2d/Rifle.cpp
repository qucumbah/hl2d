#include "Rifle.h"

Rifle::Rifle(int ownerId) : Gun(ownerId) {

}

void Rifle::pullTrigger1() {
	_startShooting();
}

void Rifle::releaseTrigger1() {
	_stopShooting();
}

void Rifle::pullTrigger2() {

}

void Rifle::releaseTrigger2() {

}

LineHitter* Rifle::_createBullet() {
	int randomAddedAngle = rand() % INACCURACY - INACCURACY / 2;
	return new LineHitter(1000, 25, 20, false, randomAddedAngle);
}

void Rifle::_startShooting() {
	_isShooting = true;
	_shoot(_createBullet(), COOLDOWN);
}

void Rifle::_stopShooting() {
	_isShooting = false;
}

void Rifle::_shoot(Hitter* hitter, int cooldown) {
	if (_isOnCooldown || !_isShooting) {
		return;
	}

	_emit("shoot", hitter);
	this->_isOnCooldown = true;
	EventQueue::after(cooldown, [this]() {
		_isOnCooldown = false;
		_shoot(_createBullet(), COOLDOWN);
	});
}
