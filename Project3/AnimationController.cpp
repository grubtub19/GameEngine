#include "AnimationController.h"
#include "Game.h"

AnimationController::AnimationController() :
	active_animation(-1),
	current_animation_timer(0),
	repeat(false)
{

}

AnimationController::~AnimationController() {

}

float AnimationController::default_ticks_per_second = 6000;

void AnimationController::update() {
	std::map<std::string, KeyStatus> keys = Game::get().input_manager.keys;
	
	//toggle animation repeat
	if (keys.at("animation_repeat_toggle").just_pressed) {
		repeat = !repeat;
	}

	//start animations
	for (int i = 0; i < animations.size(); i++) {
		
		if (Game::get().input_manager.keys.at("animation_" + std::to_string(i + 1)).just_pressed) {
			//Logger::log("start animation " + std::to_string(i));
			startAnimation(i);
		}
	}
	if (active_animation != -1) {
		increaseTime();
	}
}


bool AnimationController::startAnimation(int i) {
	if (i >= 0 && i < animations.size()) {
		Logger::log("start animation " + std::to_string(i));
		active_animation = i;
		current_animation_timer = 0;
		return true;
	}
	return false;
}

void AnimationController::increaseTime() {
	current_animation_timer += Game::get().time_manager.time_elapsed;
	if (animations.at(active_animation).duration <= current_animation_timer) {
		if (repeat) {
			startAnimation(active_animation);
		}
		else {
			active_animation = -1;
		}
	}
}

bool AnimationController::timerIsRunning() {
	if (active_animation == -1) {
		return false;
	}
	return true;
}