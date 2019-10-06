#pragma once

#include "Entity.h"

class Hitter : public Entity {
public:
	virtual void activate(double x, double y, double angle) = 0;
protected:
	int _creatorId;
	static int _nextId;
};