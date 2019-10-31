#pragma once

#include "Vec2.h"

#include <string>
#include <list>

using std::string;
using std::list;

class Level {
public:
	class Location {
	public:
		const double x, y, angle;
		Location(double _x, double _y, double _angle)
			: x(_x), y(_y), angle(_angle) {

		}
	};

	class Edge {
	public:
		const Vec2 start;
		const Vec2 body;

		const Vec2 perpendicularNormal;
		Edge(Vec2 _start, Vec2 _body)
			: start(_start),
			body(_body),
			perpendicularNormal( _body.perpendicularClockwise().normal() ) {

		}
	};

	Level(string name);
	~Level();

	string getJson();

	list<Level::Edge> getEdges();
	list<Level::Location> getSpawnLocations();
private:
	string _json;

	list<Edge> _edges;
	list<Location> _spawnLocations;

	void _addEdge(double x1, double y1, double x2, double y2);
	void _addSpawnLocation(double x, double y);
};