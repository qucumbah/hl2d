#pragma once

#include <string>

using std::string;

class Map {
public:
	class Location {
	public:
		const double x, y, angle;
		Location(double _x, double _y, double _angle)
			: x(_x), y(_y), angle(_angle) {

		}
	};

	Map();
	~Map();

	Map::Location getBestSpawnLocation();

	string getJson();
};