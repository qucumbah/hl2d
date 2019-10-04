#pragma once

#include <string>
#include <sstream>
#include <list>

#include "Map.h"

using std::string;
using std::list;

class Entity {
public:
	virtual void update(
		Map* map,
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
protected:
	string _type;
	int _id;
	double _x, _y, _angle;
	bool _renderable;
	bool _destroyed = false;
};
