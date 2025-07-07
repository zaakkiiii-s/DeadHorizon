#include "timer.h"
#include "raylib.h"
#include <iostream>
#include <string>

using namespace std;

Timer::~Timer()
{

}

Timer::Timer(int x, int y, int rad, int initTime, int speed) {
	this->initTime = initTime;
	this->x = x;
	this->y = y;
	this->rad = rad;
	permTime = initTime;
	this->speed = speed;
	counter = 0;
	timerColor = GREEN;
}

void Timer::updateTimer() {
	counter++;


	if (initTime != 0 && counter % 60 == 0) {
		initTime--;
	}


	if (initTime < (permTime / 2)) {
		timerColor = ORANGE;
	}

	if (initTime < (permTime / 3)) {
		timerColor = RED;
	}
}

int Timer::getTimeLeft() const {
	return initTime;
}

void Timer::DrawTimer() {
	DrawCircleGradient(x, y, rad, timerColor, BLACK);
	int cent = (GetScreenWidth() / 2 - rad) + (rad * 2 - MeasureText(TextFormat("%i", initTime), 30)) / 2;
	int yCent = y - rad / 3;
	DrawText(TextFormat("%i", initTime), cent, yCent, 30, BLACK);
}
