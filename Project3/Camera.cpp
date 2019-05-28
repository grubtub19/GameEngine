#define _USE_MATH_DEFINES
#include <math.h>
#include "Camera.h"
#include "data.h"
#include "Mtx.h"
#include "Game.h"
#include "Transform.h"

Camera::Camera() :
	projection_matrix(mtx::Matrix4(true)),
	sensitivity_yaw(0.1f),
	sensitivity_pitch(0.1f),
	sensitivity_roll(100.0f),
	sensitivity_move(10.0f),
	yaw(0), pitch(0), roll(0),
	transform(nullptr)
{

}

Camera::Camera(float const &fov_y, float const &aspect, float const &near_plane, float const &far_plane) :
	projection_matrix(mtx::Matrix4(true)),
	sensitivity_yaw(1.0f),
	sensitivity_pitch(1.0f),
	sensitivity_roll(100.0f),
	sensitivity_move(10.0f),
	yaw(0), pitch(0), roll(0),
	transform(nullptr)
{
	initProjectionMatrix(fov_y, aspect, near_plane, far_plane);
}

Camera::~Camera() {

}

//TODO Make this [][]
void Camera::initProjectionMatrix(float const &fov_y, float const &aspect, float const &near_plane, float const &far_plane) {
	float yScale = (float) (1.0f / tan((M_PI / 180.0f) * fov_y / 2.0f));
	float xScale = yScale / aspect;
	float nearmfar = near_plane - far_plane;
	float zScale = (near_plane + far_plane) / nearmfar;
	float Z = (2.0f * near_plane * far_plane) / nearmfar;
	projection_matrix.array[0] = xScale;
	projection_matrix.array[5] = yScale;
	projection_matrix.array[10] = zScale;
	projection_matrix.array[14] = -1.0f;
	projection_matrix.array[11] = Z;
	projection_matrix.array[15] = 0.0f;
}


mtx::Matrix4 Camera::getViewMatrix() {
	view_matrix.make_translation(-transform->position);
	view_matrix.rotate(transform->rotation.inverse());
	return view_matrix;
}

void Camera::rotateRoll(float roll) {
	this->roll += roll;
}

void Camera::rotateYawPitch(float yaw, float pitch) {
	this->yaw += yaw;
	this->pitch += pitch;
}

/**
 * Updates the containing GameObject's Transform Component's rotation based on the yaw, pitch and roll of this frame
 */
void Camera::updateRotation() {

	mtx::Quaternion yaw_quaternion = mtx::Quaternion();
	yaw_quaternion.make_rotation(mtx::Vec3(0.0f, 1.0f, 0.0f), yaw); //rotate yaw which changes the direction you are facing
	//Logger::log("Yaw: " + std::to_string(yaw) + ", Yaw Quaternion: " + yaw_quaternion.to_string());
	mtx::Quaternion pitch_quaternion = mtx::Quaternion();
	pitch_quaternion.make_rotation(mtx::Vec3(1.0f, 0.0f, 0.0f), pitch); //rotate pitch based this new facing direction
	//Logger::log("Pitch: " + std::to_string(pitch) + ", Pitch Quaternion: " + pitch_quaternion.to_string());
	mtx::Quaternion roll_quaternion = mtx::Quaternion();
	roll_quaternion.make_rotation(mtx::Vec3(0.0f, 0.0f, 1.0f), roll); //rotate pitch based this new facing direction
	//Logger::log("Roll: " + std::to_string(roll) + ", Roll Quaternion: " + roll_quaternion.to_string());
	transform->rotation *= yaw_quaternion * pitch_quaternion * roll_quaternion;
	transform->rotation.normalize();
	//Logger::log("Camera rotation: " + transform->rotation.to_string());
}

void Camera::update() {
	yaw = 0;
	pitch = 0;
	roll = 0;

	float time_elapsed = Game::get().time_manager.time_elapsed;

	if (Game::get().input_manager.keys.at("forward").is_down) {
		transform->translateLocal(0.0f, 0.0f, -sensitivity_move * time_elapsed);
		//Logger::log("Forward: " + transform->getForwardVec().to_string());
	}
	if (Game::get().input_manager.keys.at("backward").is_down) {
		transform->translateLocal(0.0f, 0.0f, sensitivity_move * time_elapsed);
	}
	if (Game::get().input_manager.keys.at("right").is_down) {
		transform->translateLocal(sensitivity_move * time_elapsed, 0.0f, 0.0f);
	}
	if (Game::get().input_manager.keys.at("left").is_down) {
		transform->translateLocal(-sensitivity_move * time_elapsed, 0.0f, 0.0f);
	}
	if (Game::get().input_manager.keys.at("up").is_down) {
		transform->translateLocal(0.0f, sensitivity_move * time_elapsed, 0.0f);
	}
	if (Game::get().input_manager.keys.at("down").is_down) {
		transform->translateLocal(0.0f, -sensitivity_move * time_elapsed, 0.0f);
	}
	if (Game::get().input_manager.keys.at("roll_left").is_down) {
		roll += Game::get().time_manager.time_elapsed * sensitivity_roll;
	}
	if (Game::get().input_manager.keys.at("roll_right").is_down) {
		roll -= Game::get().time_manager.time_elapsed * sensitivity_roll;
	}
	yaw += sensitivity_yaw * Game::get().input_manager.mouse_movement.x;
	pitch += sensitivity_pitch * Game::get().input_manager.mouse_movement.y;
	
	updateRotation();

	//Logger::log("Camera position: " + transform->position.to_string());
}