#pragma once
#include "Entity.h"
class LaserPointer : public Entity {
public:
	LaserPointer(int x, int y);
	virtual void update(
		Level* level,
		list<Entity*>* entities,
		string additionalInfo);
};

