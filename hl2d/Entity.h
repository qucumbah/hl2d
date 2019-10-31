#pragma once

#include <string>
#include <sstream>
#include <list>

#include "Level.h"

using std::string;
using std::list;

class Entity {
public:
	virtual void update(
		Level* level,
		list<Entity*>* otherEntities,
		string additionalInfo) = 0;

	virtual string getJson() {
		return "{\"renderable\": \"false\"}";
	};

	string getType() {
		return _type;
	}

	bool isRenderable() {
		return _renderable;
	}
	bool isDestroyed() {
		return _destroyed;
	}

	int getId() {
		return _id;
	}

	void destroy() {
		_destroyed = true;
	}

	double getX() {
		return _x;
	}
	double getY() {
		return _y;
	}
	double getAngle() {
		return _angle;
	}
	double getRadius() {
		return _radius;
	}

	Vec2 getPosition() {
		return Vec2(_x, _y);
	}
protected:
	string _type;
	int _id;
	double _x, _y, _angle, _radius;
	bool _renderable;
	bool _destroyed = false;
};
