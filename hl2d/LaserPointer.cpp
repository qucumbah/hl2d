#include "LaserPointer.h"

LaserPointer::LaserPointer(int x, int y) {
	_type = "LaserPointer";
	_renderable = true;
	_id = getId();
	_x = x;
	_y = y;
}

void LaserPointer::update(
	Level* level,
	list<Entity*>* entities,
	string additionalInfo
) {
	_renderable = false;
	destroy();
}