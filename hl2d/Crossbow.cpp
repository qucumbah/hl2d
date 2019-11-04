#include "Crossbow.h"

Crossbow::Crossbow(int ownerId) : Gun(ownerId) {

}

void Crossbow::pullTrigger1() {
	_shoot(new Projectile(
		MAX_DISTANCE,
		SPEED,
		DAMAGE,
		DAMAGE,
		true,
		0
	), COOLDOWN);
}

void Crossbow::releaseTrigger1() {

}

void Crossbow::pullTrigger2() {

}

void Crossbow::releaseTrigger2() {

}
