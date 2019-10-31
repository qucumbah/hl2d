#pragma once

#include <functional>
#include <map>
#include <thread>
#include <chrono>

#include <Windows.h>
#include <iostream>

using std::function;
using std::map;
using std::thread;
using namespace std::chrono;

class EventQueue {
public:
	static void initialize(int precision = 5) {
		_precision = precision;
		_start = steady_clock::now();
		_now = _start;
		_lastId = 0;

		thread* t = new thread(_inner);
		t->detach();
	}

	static int after(int time, function<void()> action) {
		actionInfo info;
		info.action = action;
		info.executionTime = _now + milliseconds(time);

		info.executeOnce = true;

		int id = _lastId++;
		_actions[id] = info;

		return id;
	}

	static int every(int time, function<void()> action) {
		actionInfo info;
		info.action = action;
		info.executionTime = _now + milliseconds(time);

		info.executeOnce = false;
		info.period = time;

		int id = _lastId++;
		_actions[id] = info;

		return id;
	}
private:
	//Block constructor
	EventQueue() {}

	struct actionInfo {
		function<void()> action;
		steady_clock::time_point executionTime;

		bool executeOnce;
		int period;
	};

	static int _precision;

	static steady_clock::time_point _start;
	static steady_clock::time_point _now;

	static map<int, actionInfo> _actions;
	static int _lastId;

	static void _inner() {
		while (true) {
			Sleep(_precision);
			_now = steady_clock::now();

			auto actionsCopy = _actions;
			_actions.clear();
			auto checkedActions = _check(actionsCopy);
			_actions.merge(checkedActions);
		}
	}

	static map<int, actionInfo> _check(map<int, actionInfo> actions) {
		auto start = actions.begin();
		auto end = actions.end();

		for (auto it = start; it != end; ) {
			auto& [id, actionContainer] = *it;
			auto [action, executionTime, executeOnce, period] = actionContainer;

			if (_now >= executionTime) {
				//Queue next execution
				actionContainer.executionTime += milliseconds(period);
				action();

				if (executeOnce) {
					actions.erase(it++);
				}
				else {
					it++;
				}
			}
			else {
				it++;
			}
		}

		return actions;
	}
};