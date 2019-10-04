#pragma once

#include <functional>

class Timer {
private:
	bool stopped = false;
	int delay;
	std::function<void()> func;
	void inner();
public:
	Timer(std::function<void()> func, int delay);
	~Timer();

	void stop();
};

