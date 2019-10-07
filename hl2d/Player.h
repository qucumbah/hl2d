#pragma once

#include "Entity.h"
#include "Map.h"
#include "Gun.h"
#include "Hitter.h"
#include "Vec2.h"

#include <string>

using std::string;

class Player : public Entity {
public:
	static const int CONNECTING = 0;
	static const int CONNECTED = 1;
	static const int DISCONNECTING = 2;
	static const int DISCONNECTED = 3;

	static const int MAX_HEALTH = 100;
	static const int MOVEMENT_SPEED = 1;
	static const int PLAYER_RADIUS = 8;

	Player(int id);
	~Player();

	string getName();
	void connectWithName(string name);

	virtual void update(
		Map* map,
		list<Entity*>* entities,
		string playerInputsString) override;

	string getJson() override;

	int getConnectionStatus();

	bool isAlive();

	void hit(int amount);
	void respawn(double x, double y);

	void shoot(Hitter* hitter);
private:
	int _connectionStatus;
	string _name;
	int _crosshairX;
	int _crosshairY;
	Gun** _guns;
	int _currentGun;
	int _health;

	list<Hitter*> _shotHitters;

	bool _up;
	bool _right;
	bool _down;
	bool _left;

	void _updateCrosshairPosition(int x, int y);
	void _handleAction(int actionCode);
	void _updateRotation();

	Vec2 _getMovement();
	Vec2 _getLargestBounce(Vec2 movement, Map* map);
	Vec2 _getBounce(Vec2 playerPosition, Map::Edge edge);
	void _move(Vec2 movement);

	//stability test
	int _lastNumber = 0;
};
