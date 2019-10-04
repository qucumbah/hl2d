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

class A {
public:
	A(int arg) {
		_arg = arg;
	}

	int getArg() {
		return _arg;
	}
private:
	int _arg;
};

int main() {
	//Timer t([]() { cout << "test" << endl; }, 1000);
	//Sleep(5000);
	//ServerAdapter s;

	/*
	string s = util::SHA1("ViFipDSLZ5fyHuVf3PFDBQ==258EAFA5-E914-47DA-95CA-C5AB0DC85B11");
	cout << s << endl;
	string b = util::base64Encode(s);
	cout << b << endl;
	*/
	/*
	EventEmitter<int, string> e;
	e.on("e", handler);
	e._emit("e", 15, " askj");
	*/
	
	util::startWinsock();
	ServerAdapter s;
	/*
	list<A*> list;
	list.push_back(new A(15));
	list.push_back(new A(10));
	list.push_back(new A(5));

	for (auto a : list) {
		cout << a->getArg() << endl;
	}

	list<A*> list2 = list;
	*/
	system("pause");
	return 0;
}