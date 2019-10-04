#pragma once

#include "Gun.h"
#include "AreaHitter.h"

class Crowbar : public Gun {
public:
	Crowbar(int ownerId);

	void pullTrigger1() override;
	void releaseTrigger1() override;
	void pullTrigger2() override;
	void releaseTrigger2() override;
};
