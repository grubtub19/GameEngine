#pragma once
#include <unordered_map>
#include <string>
#include <assimp/scene.h>
#include "data.h"
#include "Component.h"

class AnimationController: public Component {
public:
	AnimationController();
	~AnimationController();
	
	std::vector<Animation> animations;

	int active_animation;
	bool repeat;
	float current_animation_timer;

	static float default_ticks_per_second;

	bool startAnimation(int i);

	void increaseTime();

	bool timerIsRunning();

	void update();
};