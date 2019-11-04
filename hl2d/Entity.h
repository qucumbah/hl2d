#pragma once

#include <string>
#include <sstream>
#include <list>

#include "Level.h"
#include "util.h"

using std::to_string;
using std::string;
using std::list;

class Entity {
public:
	virtual void update(
		Level* level,
		list<Entity*>* otherEntities,
		string additionalInfo) = 0;

	virtual string getJson() {
		map<string, string> pairs;

		pairs["renderable"] = _renderable ? "true" : "false";
		pairs["type"] = _type;
		pairs["angle"] = to_string(_angle);
		pairs["radius"] = to_string(_radius);
		pairs["x"] = to_string(_x);
		pairs["y"] = to_string(_y);
		pairs["id"] = to_string(_id);

		string json = util::createJson(pairs);
		return json;
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
