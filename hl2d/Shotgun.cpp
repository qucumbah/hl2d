#include "Shotgun.h"

Shotgun::Shotgun(int ownerId) : Gun(ownerId) {

}

void Shotgun::pullTrigger1() {
	list<LineHitter*> pellets;
	for (int i = -3; i <= 3; i++) {
		pellets.push_back(
			new LineHitter(250, 15, 5, false, i)
		);
	}

	_shootPellets(pellets, COOLDOWN);
}

void Shotgun::releaseTrigger1() {

}

void Shotgun::pullTrigger2() {
	list<LineHitter*> pellets;
	for (int i = -6; i <= 6; i++) {
		pellets.push_back(
			new LineHitter(250, 15, 5, false, i / 2.0)
		);
	}

	_shootPellets(pellets, COOLDOWN * 2);
}

void Shotgun::releaseTrigger2() {

}

void Shotgun::_shootPellets(list<LineHitter*> pellets, int cooldown) {
	if (_isOnCooldown) {
		return;
	}

	for (auto pellet : pellets) {
		_emit("shoot", pellet);
	}

	_isOnCooldown = true;
	EventQueue::after(cooldown, [this]() { this->_isOnCooldown = false; });
}