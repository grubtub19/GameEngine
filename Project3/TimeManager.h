#pragma once

class TimeManager {
public:
	static bool instantiated;
	TimeManager();

	float time_elapsed;
	float current_time;

	void update();
};