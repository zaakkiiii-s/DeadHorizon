#ifndef TIMER_H
#define TIMER_H

#include "raylib.h"  

class Timer {
public:
	Timer(int x, int y, int rad, int initTime, int speed);
	~Timer();

	void updateTimer();
	void DrawTimer();
	int getTimeLeft() const;

private:
	int initTime;
	int permTime;
	int speed;
	int counter;
	int x, y, rad;
	Color timerColor;
};

#endif
