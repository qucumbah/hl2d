#include <iostream>
#include <functional>
#include <list>
#include "Timer.h"
#include "Socket.h"
#include "ServerAdapter.h"
#include "Vec2.h"

#include "util.h"
#include "EventQueue.h"
#include "EventEmitter.h"

int main() {
	util::startWinsock();
	EventQueue::initialize();

	ServerAdapter s;

	system("pause");
	return 0;
}