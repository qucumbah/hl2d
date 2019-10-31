#pragma once

#include "Entity.h"
//#include "Player.h"
class Player; //C++ is weird man

class Hitter : public Entity {
public:
	//Hitter gets activated by player in shoot() method, at the same time as
	//he queues this hitter to be shot on the next tick
	virtual void activate(Player* player) = 0;
	virtual Vec2 getDirectionNormal() {
		return Vec2( cos(_angle), sin(_angle) );
	}
protected:
	int _creatorId;
	int _damage;
	static int _nextId;
};