#include <iostream>
#include <functional>
#include <list>
#include "Timer.h"
#include "Socket.h"
#include "ServerAdapter.h"
#include "Vec2.h"

#include "util.h"
#include "EventEmitter.h"

using namespace std;

int main() {
	util::startWinsock();
	ServerAdapter s;

	system("pause");
	return 0;
}