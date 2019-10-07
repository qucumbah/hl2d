#include "Map.h"
#include "util.h"

#include <string>
#include <fstream>
#include <map>

using std::string;
using std::ifstream;
using std::map;
using std::cout;
using std::endl;
using std::to_string;

Map::Map(string mapName) {
	ifstream mapFile;
	string path = "maps/" + mapName + ".txt";
	mapFile.open(path);

	if (!mapFile.is_open()) {
		throw std::invalid_argument("Couldn't open map file: " + path);
	}

	string token;
	int x1, y1, x2, y2;

	list<string> walls;

	while (mapFile >> token) {
		if (token == "wall") {
			mapFile >> x1 >> y1 >> x2 >> y2;
			//x1,y1 is top left, x2,y2 is bottom right
			if (x1 > x2 || y1 > y2) {
				throw std::invalid_argument("Map file has invalid format");
			}

			//Create JSON representation of a wall
			map<string, string> jsonPairs;
			jsonPairs["x"] = to_string(x1);
			jsonPairs["y"] = to_string(y1);
			jsonPairs["width"] = to_string(x2 - x1);
			jsonPairs["height"] = to_string(y2 - y1);
			walls.push_back( util::createJson(jsonPairs) );

			//Add actual game bounds
			//All perpendiculars face inside for simpler movement calculation
			/*
			_addEdge(x1, y1, x2, y1); //Top
			_addEdge(x2, y1, x2, y2); //Right
			_addEdge(x2, y2, x1, y2); //Bottom
			_addEdge(x1, y2, x1, y1); //Left
			*/
			_addEdge(x1, y1, x1, y2);
			_addEdge(x1, y2, x2, y2);
			_addEdge(x2, y2, x2, y1);
			_addEdge(x2, y1, x1, y1);
		}
		else if (token == "spawnLocation") {
			mapFile >> x1 >> y1;

			//Dont need to visually represent spawn locations
			_addSpawnLocation(x1, y1);
		}
		else {
			throw std::invalid_argument("Map file has invalid format");
		}
	}

	_json = util::createJsonArray(walls);

	mapFile.close();

	//Can support diagonal walls; might use later
	//_edges.push_back(Map::Edge(Vec2(100, 100), Vec2(50, 50)));
}

Map::~Map() {

}

string Map::getJson() {
	return _json;
}

list<Map::Edge> Map::getEdges() {
	return _edges;
}

list<Map::Location> Map::getSpawnLocations() {
	return _spawnLocations;
}

void Map::_addEdge(double x1, double y1, double x2, double y2) {
	Vec2 start(x1, y1);
	Vec2 body(x2 - x1, y2 - y1);

	cout << start << body << endl;

	_edges.push_back( Map::Edge(start, body) );
}

void Map::_addSpawnLocation(double x, double y) {
	_spawnLocations.push_back( Map::Location(x, y, 0) );
}