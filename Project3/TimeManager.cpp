#include <assert.h>
#include "TimeManager.h"
#include "GLFWManager.h"
#include "Logger.h"

bool TimeManager::instantiated = false;

TimeManager::TimeManager() :
	time_elapsed(0),
	current_time((float)glfwGetTime())
{
	assert(!instantiated);
	instantiated = true;
}

void TimeManager::update() {
	float new_current_time = (float)glfwGetTime();
	time_elapsed = new_current_time - current_time;
	current_time = new_current_time;
	//Logger::log("Time elapsed" + std::to_string(time_elapsed));
}