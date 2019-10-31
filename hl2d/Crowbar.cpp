#include "Crowbar.h"

Crowbar::Crowbar(int ownerId) : Gun(ownerId) {

}

void Crowbar::pullTrigger1() {
	_emit("shoot", new AreaHitter(25, 50));
}

void Crowbar::releaseTrigger1() {

}

void Crowbar::pullTrigger2() {

}

void Crowbar::releaseTrigger2() {

}
