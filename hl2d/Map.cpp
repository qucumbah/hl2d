#include "Map.h"

#include <string>

using std::string;

Map::Map() {

}

Map::~Map() {

}

Map::Location Map::getBestSpawnLocation() {
	double x = rand() % 30 + 15;
	double y = rand() % 30 + 15;
	double angle = 0;
	Map::Location result(x, y, angle);

	return result;
}

string Map::getJson() {
	return "[[0, 0], [1, 1]]";
}