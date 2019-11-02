#pragma once

#include "Gun.h"
#include "LineHitter.h"

#include <list>

using std::list;

class Shotgun : public Gun {
public:
	static const int COOLDOWN = 500;

	Shotgun(int ownerId);

	void pullTrigger1() override;
	void releaseTrigger1() override;
	void pullTrigger2() override;
	void releaseTrigger2() override;

protected:
	void _shootPellets(list<LineHitter*> pellets, int cooldown);
};
