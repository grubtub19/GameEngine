#pragma once
#include "Component.h"
#include "Mtx.h"

class Transform;

class Camera: public Component {
public:
	Camera();
	Camera(float const &fovX, float const &aspect, float const &near_plane, float const &far_plane);
	~Camera();

	mtx::Matrix4 projection_matrix;
	mtx::Matrix4 view_matrix;

	Transform* getTransform();

	float sensitivity_yaw;
	float sensitivity_pitch;
	float sensitivity_roll;
	float sensitivity_move;

	void rotateRoll(float roll);
	void rotateYawPitch(float yaw, float pitch);

	mtx::Matrix4 getViewMatrix();
	void initProjectionMatrix(float const& fovX, float const& aspect, float const& near_plane, float const& far_plane);


private:
	float yaw, pitch, roll;
	void updateRotation();

public:
	void update();
};