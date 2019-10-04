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
	_x = 0;
	_y = 0;
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
	_move();
}

string Player::getJson() {
	map<string, string> pairs;

	pairs["renderable"] = _renderable?"1":""; //Yes
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
	cout << amount << endl;
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
	hitter->activate(_x, _y, _angle);
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

	_angle = (y>0) ? acos(x/r) : PI-acos(x/r);
}

void Player::_move() {

}