#pragma once

#include "Entity.h"
//#include "Player.h"
class Player; //C++ is weird man

class Hitter : public Entity {
public:
	virtual void activate(Player* player) = 0;
	virtual Vec2 getDirectionNormal() {
		return Vec2( cos(_angle), sin(_angle) );
	}
protected:
	int _creatorId;
	static int _nextId;
};