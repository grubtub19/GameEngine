#pragma once
#include "Component.h"
#include "data.h"

class Transform;
class AnimationController;

class Skeleton : public Component {
public:
	Skeleton();
	~Skeleton();

	Transform* transform;
	AnimationController* animation_controller;

	std::vector<AnimationKeyFrames> animations;

	void animatePosition(AnimationKeyFrames& key_frames, float curr_time);
	void animateRotation(AnimationKeyFrames& key_frames, float curr_time);
	void animateScale(AnimationKeyFrames& key_frames, float curr_time);

	void update();
};