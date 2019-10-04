#include "Timer.h"
#include <thread>
#include <Windows.h>

using namespace std;

Timer::Timer(function<void()> func, int delay) {
	this->func = func;
	this->delay = delay;

	thread t( bind(&Timer::inner, this) );
	t.detach();
}

Timer::~Timer() {
	this->stop();
}

void Timer::inner() {
	while (true) {
		Sleep(this->delay);

		if (this->stopped) {
			return;
		}

		this->func();
	}
}

void Timer::stop() {
	this->stopped = true;
}