#pragma once

#include "Gun.h"
#include "LineHitter.h"

class Rifle : public Gun {
public:
	Rifle(int ownerId);

	void pullTrigger1() override;
	void releaseTrigger1() override;
	void pullTrigger2() override;
	void releaseTrigger2() override;
};
