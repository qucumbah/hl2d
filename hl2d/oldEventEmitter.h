//This is a better version of current EventEmitter. Unfortunately it couldn't
//be used because _listeners map would all of a sudden become garbage in the
//middle of execution. Calling test() after adding one handler would show that
//it's length is 1, just as it should. But as soon as _emit gets called it
//becomes random. Tried everything, spent ~10 hours in total and gave up.
//Fortunately this project doesn't have any situations where more than one
//handler is needed for an event.

#pragma once
#include <functional>
#include <string>
#include <map>
#include <list>

using std::string;
using std::list;
using std::function;
using std::map;

template <typename... T>
class EventEmitter {
public:
	EventEmitter() {
		map<string, list< function<void(T...)> >*> m;
		std::cout << "creating eventEmitter " << m.size() << std::endl;
		_listeners = m;
		std::cout << "created eventEmitter " << _listeners.size() << std::endl;
	}

	void on(string eventName, function<void(T...)> handler) {
		if (!_listeners[eventName]) {
			_listeners[eventName] = new list< function<void(T...)> >;
		}
		_listeners[eventName]->push_back(handler);
		std::cout << "added event listener; " << _listeners.size() << std::endl;
	}
//protected:
	map<string, list< function<void(T...)> >*> _listeners;

	void test() {
		std::cout << "testing" << _listeners.size() << std::endl;
	}

	void _emit(string eventName, T... data) {
		test();
		//std::cout << "emitting" << eventName << _listeners.size() << std::endl;
		/*
		if (!_listeners[eventName]) {
			return;
		}
		for (auto handler : *(_listeners[eventName])) {
			handler(data...);
		}
		*/
	}
//private:
};
