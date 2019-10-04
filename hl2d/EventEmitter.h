#pragma once
#include <iostream>
#include <functional>
#include <string>
#include <map>
#include <list>

using std::string;
using std::function;
using std::map;
using std::cout;
using std::endl;

template <typename... T>
class EventEmitter {
public:
	void on(string eventName, function<void(T...)> handler) {
		_listeners[eventName] = handler;
	}
//protected:
	map<string, function<void(T...)>> _listeners;

	void _emit(string eventName, T... data) {
		if (!_listeners[eventName]) {
			return;
		}
		_listeners[eventName](data...);
	}
//private:
};
