#include "Skeleton.h"
#include "AnimationController.h"
#include "Transform.h"
#include "Logger.h"

Skeleton::Skeleton() 
{

}

Skeleton::~Skeleton() 
{

}

void Skeleton::update() {
	//Logger::log("skeleton update");
	//If there is an animation playing
	int active_animation = animation_controller->active_animation;
	//Logger::log("active animation: " + std::to_string(active_animation));
	if (animation_controller->timerIsRunning() && animations.at(active_animation).is_used) {
		
		//Get the list of key frames for the current animation
		AnimationKeyFrames& key_frames = animations.at(active_animation);
		//Get the timer for the current animation
		float curr_time = animation_controller->current_animation_timer;
		//Logger::log("curr_time: " + std::to_string(curr_time));
		animatePosition(key_frames, curr_time);
		animateRotation(key_frames, curr_time);
		animateScale(key_frames, curr_time);
	}
	//Logger::log("skeleton finished");
}

void Skeleton::animatePosition(AnimationKeyFrames& key_frames, float curr_time) {
	//If there is only 1 key frame, use it
	if (key_frames.position_key_frames.size() == 1) {
		transform->position = key_frames.position_key_frames.at(0).position;
	}
	else {
		//Find the 2 key frames with time stamps between the current time
		//chronologically check every keyframe to find one that is past the current time
		for (int i = 1; i < key_frames.position_key_frames.size(); i++) {
			PositionKeyFrame& second = key_frames.position_key_frames.at(i);
			//if past the current time
			//Logger::log("keyframe timestamp: " + std::to_string(second.time_stamp));
			if (second.time_stamp > curr_time) {
				//the previous keyframe is the left one
				PositionKeyFrame& first = key_frames.position_key_frames.at(i - 1);
				//time between keyframes
				float delta_time = second.time_stamp - first.time_stamp;
				float lerp_time = (curr_time - first.time_stamp) / delta_time;
				//lerp between the two keyframes and assign it as the transform's position
				transform->position = first.position.lerp(second.position, lerp_time);
				//Logger::log("Left: " + first.position.to_string() + ", " + std::to_string(first.time_stamp));
				//Logger::log("Right: " + second.position.to_string() + ", " + std::to_string(second.time_stamp));
				//Logger::log("Position: " + transform->position.to_string());
				break;
			}
		}
	}
}

void Skeleton::animateRotation(AnimationKeyFrames& key_frames, float curr_time) {
	//If there is only 1 key frame, use it
	if (key_frames.rotation_key_frames.size() == 1) {
		transform->rotation = key_frames.rotation_key_frames.at(0).rotation;
	}
	else {
		//Find the 2 key frames with time stamps between the current time
		//chronologically check every keyframe to find one that is past the current time
		for (int i = 1; i < key_frames.rotation_key_frames.size(); i++) {
			RotationKeyFrame& second = key_frames.rotation_key_frames.at(i);
			//Logger::log("keyframe timestamp: " + std::to_string(second.time_stamp));
			//if past the current time
			if (second.time_stamp > curr_time) {
				//the previous keyframe is the left one
				RotationKeyFrame& first = key_frames.rotation_key_frames.at(i - 1);
				//time between keyframes
				float delta_time = second.time_stamp - first.time_stamp;
				float lerp_time = (curr_time - first.time_stamp) / delta_time;
				//lerp between the two keyframes and assign it as the transform's position
				transform->rotation = first.rotation.slerp(second.rotation, lerp_time);
				//Logger::log("Left: " + first.rotation.to_string() + ", " + std::to_string(first.time_stamp));
				//Logger::log("Right: " + second.rotation.to_string() + ", " + std::to_string(second.time_stamp));
				//Logger::log("Rotation: " + transform->rotation.to_string());
				break;
			}
		}
	}
}

void Skeleton::animateScale(AnimationKeyFrames& key_frames, float curr_time) {
	//If there is only 1 key frame, use it
	if (key_frames.scale_key_frames.size() == 1) {
		transform->scaler = key_frames.scale_key_frames.at(0).scale;
	}
	else {
		//Find the 2 key frames with time stamps between the current time
		//chronologically check every keyframe to find one that is past the current time
		for (int i = 1; i < key_frames.scale_key_frames.size(); i++) {
			ScaleKeyFrame& second = key_frames.scale_key_frames.at(i);
			//Logger::log("keyframe timestamp: " + std::to_string(second.time_stamp));
			//if past the current time
			if (second.time_stamp > curr_time) {
				//the previous keyframe is the left one
				ScaleKeyFrame& first = key_frames.scale_key_frames.at(i - 1);
				//time between keyframes
				float delta_time = second.time_stamp - first.time_stamp;
				float lerp_time = (curr_time - first.time_stamp) / delta_time;
				//lerp between the two keyframes and assign it as the transform's position
				transform->scaler = first.scale.lerp(second.scale, lerp_time);
				//Logger::log("Left: " + first.scale.to_string() + ", " + std::to_string(first.time_stamp));
				//Logger::log("Right: " + second.scale.to_string() + ", " + std::to_string(second.time_stamp));
				//Logger::log("Scale: " + transform->scaler.to_string());
				break;
			}
		}
	}
}