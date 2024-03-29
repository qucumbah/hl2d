#pragma once

#include "EventEmitter.h"
#include "Hitter.h"
#include "EventQueue.h"

#include <string>

using std::string;

class Gun : public EventEmitter<Hitter*> {
public:
	Gun(int ownerId) {
		_ownerId = ownerId;
	}

	virtual void pullTrigger1() = 0;
	virtual void releaseTrigger1() = 0;
	virtual void pullTrigger2() = 0;
	virtual void releaseTrigger2() = 0;

	int getOwnerId() {
		return _ownerId;
	}
protected:
	int _ownerId;
	bool _isOnCooldown = false;

	virtual void _shoot(Hitter* hitter, int cooldown) {
		if (_isOnCooldown) {
			return;
		}

		_emit("shoot", hitter);
		_isOnCooldown = true;
		EventQueue::after(cooldown, [this]() { this->_isOnCooldown = false; });
	}
};
