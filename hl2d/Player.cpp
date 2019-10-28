#include "Player.h"

#include <iostream>
#include "util.h"

#include "Crowbar.h"
#include "Rifle.h"

using std::string;
using std::stringstream;
using std::cout;
using std::endl;
using std::to_string;
using std::exception;

//Constants for player's actions; the release actions have the same IDs but they
//are negative, e.g. click is encoded as:
//11, -11 (attack1 pressed, attack1 released)
const int UP = 1;
const int RIGHT = 2;
const int DOWN = 3;
const int LEFT = 4;

const int GUN1 = 5;
const int GUN2 = 6;
const int GUN3 = 7;
const int GUN4 = 8;
const int GUN5 = 9;
const int GUN6 = 10;

const int ATTACK1 = 11;
const int ATTACK2 = 12;

Player::Player(int id) {
	_connectionStatus = CONNECTING;
	_id = id;
	_type = "Player";
	_x = 100;
	_y = 100;
	_angle = 0;
	_renderable = false;

	_up = false;
	_right = false;
	_down = false;
	_left = false;

	_guns = new Gun*[6];
	_guns[0] = new Crowbar(id);
	_guns[1] = new Rifle(id);

	for (int i = 0; i < 2; i++) {
		_guns[i]->on("shoot", [this](Hitter* hitter) {
			this->shoot(hitter);
		});
	}

	_currentGun = 0;
	_health = 100; ///Dead on creation
}

Player::~Player() {
	_connectionStatus = DISCONNECTED;
}

string Player::getName() {
	return _name;
}

void Player::connectWithName(string name) {
	_connectionStatus = CONNECTED;
	_renderable = true;
	_name = name;
}

void Player::update(Map* map,
	list<Entity*>* entities,
	string playerInputsString) {
	if (!isAlive()) {
		return;
	}

	stringstream tokenStream(playerInputsString);
	string token;

	
	while (tokenStream >> token) {
		if (token == "crosshair") {
			int x, y;

			tokenStream >> x;
			tokenStream >> y;
			if (tokenStream.fail()) {
				continue;
			}

			_updateCrosshairPosition(x, y);
		}
		/*
		else if (token == "stabilityTest") {
			//stability test
			int number;
			tokenStream >> number;
			if (number != _lastNumber) {
				cout << "collision: " << number << ", " << _lastNumber << endl;
				_lastNumber = number;
			}
			else {
				cout << "ok: " << number << ", " << _lastNumber << endl;
			}
			_lastNumber++;
		}
		*/
		else {
			int actionCode = atoi( token.data() );
			//atoi returns 0 if there was an error; We dont have action with
			//code 0 so we can be sure that it's an input error
			if (actionCode == 0) {
				continue;
			}

			_handleAction(actionCode);
		}
	}

	for (auto hitter : _shotHitters) {
		entities->push_back(hitter);
	}
	_shotHitters.clear();

	_updateRotation();

	Vec2 movement = _getMovement();
	Vec2 bounce = _getLargestBounce(movement, map);

	_move(movement + bounce);
}

string Player::getJson() {
	map<string, string> pairs;

	pairs["renderable"] = _renderable?"true":"false"; //Yes
	pairs["connectionStatus"] = to_string(_connectionStatus);
	pairs["type"] = _type;
	pairs["name"] = _name;
	pairs["id"] = to_string(_id);
	pairs["angle"] = to_string(_angle);
	pairs["x"] = to_string(_x);
	pairs["y"] = to_string(_y);
	pairs["currentGun"] = to_string(_currentGun);
	pairs["health"] = to_string(_health);

	string json = util::createJson(pairs);
	return json;
}

int Player::getConnectionStatus() {
	return _connectionStatus;
}

bool Player::isAlive() {
	return _health>0;
}

void Player::hit(int amount) {
	_health = _health - amount;
	if (_health < 0) {
		_health = 0;
	}
}

void Player::respawn(double x, double y) {
	_x = x;
	_y = y;

	_health = MAX_HEALTH;
}

void Player::shoot(Hitter* hitter) {
	hitter->activate(this);
	_shotHitters.push_back(hitter);
}

void Player::_updateCrosshairPosition(int x, int y) {
	_crosshairX = x;
	_crosshairY = y;
}

void Player::_handleAction(int actionCode) {
	switch (actionCode) {
	//Save all controlls, later they will be used to calculate movement vector
	case +UP:
		_up = true;
		break;
	case +RIGHT:
		_right = true;
		break;
	case +DOWN:
		_down = true;
		break;
	case +LEFT:
		_left = true;
		break;
	case -UP:
		_up = false;
		break;
	case -RIGHT:
		_right = false;
		break;
	case -DOWN:
		_down = false;
		break;
	case -LEFT:
		_left = false;
		break;
	//Pull or release triggers of current gun, which will handle these actions
	case +ATTACK1:
		_guns[_currentGun]->pullTrigger1();
		break;
	case -ATTACK1:
		_guns[_currentGun]->releaseTrigger1();
		break;
	case +ATTACK2:
		_guns[_currentGun]->pullTrigger2();
		break;
	case -ATTACK2:
		_guns[_currentGun]->releaseTrigger2();
		break;
	//Only need +events here
	case +GUN1:
		_currentGun = 0;
		break;
	case +GUN2:
		_currentGun = 1;
		break;
	case +GUN3:
		_currentGun = 2;
		break;
	case +GUN4:
		_currentGun = 3;
		break;
	case +GUN5:
		_currentGun = 4;
		break;
	case +GUN6:
		_currentGun = 5;
		break;
	}
}

void Player::_updateRotation() {
	//Crosshair position relative to player
	int x = _crosshairX - _x;
	int y = _crosshairY - _y;

	//Distance from player to crosshair
	double r = sqrt(x*x + y*y);
	const double PI = 3.14;

	_angle = (y>0) ? acos(x/r) : 2*PI-acos(x/r);
}

Vec2 Player::_getMovement() {
	if (!_up && !_down && !_right && !_left) {
		return Vec2(0, 0);
	}

	int dx = 0;
	int dy = 0;
	if (_right) dx += 1;
	if (_left)  dx -= 1;
	if (_down)  dy += 1;
	if (_up)    dy -= 1;

	Vec2 movement = Vec2(dx, dy).normal() * MOVEMENT_SPEED;

	return movement;
}

Vec2 Player::_getLargestBounce(Vec2 movement, Map* map) {
	//Here we need to find the largest bounce
	//We need it because after the player has moved he can be inside an edge
	//and we need to push him out by 'bouncing' him back
	//We only need the largest one because we can deal with the rest on the
	//next ticks; If we try to bounce all of them at the same time we can get
	//into an infinite loop (e.g. player is pinched between 2 walls)
	Vec2* largestBounce = new Vec2(0, 0); //For comparisons
	Vec2 playerPosition = Vec2(_x, _y) + movement;

	list<Map::Edge> edges = map->getEdges();

	for (auto edge : edges) {
		Vec2 bounce = _getBounce(playerPosition, edge);

		//cout << bounce << *largestBounce << (bounce > *largestBounce) << endl;

		if (bounce > *largestBounce) {
			largestBounce = new Vec2(bounce.x, bounce.y);
			//cout << "replaced; new largest bounce: " << *largestBounce << endl;
		}
	}

	return *largestBounce;
}

Vec2 Player::_getBounce(Vec2 playerPosition, Map::Edge edge) {
	//Algorithm of finding a bounce is described in sketch 1
	Vec2 n = edge.perpendicularNormal * PLAYER_RADIUS;

	if (Vec2::isFacing(playerPosition, n, edge.start, edge.body)) {
		double k = Vec2::getIntersectionCoefficient(
			playerPosition, n, edge.start, edge.body
		);

		//cout << "is facing; ";

		if (k >= 1 || k <= 0) {
			//Not inside or too much inside
			//cout << "Not inside or too much inside" << endl;
			return Vec2(0, 0);
		}

		//cout << "Touching edge; result: " << n * (k - 1) << endl;

		return n * (k - 1);
	}
	else {
		//This is not shown in sketch 1, but it should be obvious that
		//w is the closest end point of an edge relative to player
		//so d is the distance from player to the closest end point of the edge
		Vec2 d1 = edge.start - playerPosition;
		Vec2 d2 = edge.start + edge.body - playerPosition;
		Vec2 d = (d1 < d2) ? d1 : d2;

		//cout << "isn't facing; ";

		if (d.length() > PLAYER_RADIUS) {
			//Not touching any end point of the edge
			//cout << "Not touching any end point of the edge" << endl;
			return Vec2(0, 0);
		}

		Vec2 r = d.normal() * PLAYER_RADIUS;

		//cout << "Touching end of the edge; result: " << d - r << endl;

		return d - r;
	}
}

void Player::_move(Vec2 movement) {
	if (movement.x != 0 && movement.y != 0) {
		//cout << movement << endl;
	}
	_x += movement.x;
	_y += movement.y;
}