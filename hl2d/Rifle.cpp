#include "Rifle.h"

Rifle::Rifle(int ownerId) : Gun(ownerId) {

}

void Rifle::pullTrigger1() {
	_emit("shoot", new LineHitter(250, 25, 20, false));
}

void Rifle::releaseTrigger1() {

}

void Rifle::pullTrigger2() {

}

void Rifle::releaseTrigger2() {

}
