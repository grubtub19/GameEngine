#include <assert.h>
#include "Transform.h"
#include "Logger.h"
#include "Game.h"

Transform::Transform() :
	position(mtx::Vec3(0,0,0)),
	rotation(mtx::Quaternion()),
	scaler(mtx::Vec3(1.0f, 1.0f, 1.0f)),
	local_transformation_matrix(mtx::Matrix4(true)),
	global_transformation_matrix(mtx::Matrix4(true))
{

}

Transform::Transform(float x, float y, float z) :
	Component(),
	position(mtx::Vec3(x, y, z)),
	rotation(mtx::Quaternion()),
	scaler(mtx::Vec3(1.0f, 1.0f, 1.0f)),
	local_transformation_matrix(mtx::Matrix4(true)),
	global_transformation_matrix(mtx::Matrix4(true))
{

}

Transform::Transform(mtx::Vec3 const& position) :
	position(position),
	rotation(mtx::Quaternion()),
	scaler(mtx::Vec3(1.0f, 1.0f, 1.0f)),
	local_transformation_matrix(mtx::Matrix4(true)),
	global_transformation_matrix(mtx::Matrix4(true))
{
	
}

Transform::~Transform()
{

}

std::string Transform::toString() {
	return std::string(
		"Transform: ") +
		"\n    Position: " + position.to_string() +
		"\n    Rotation: " + rotation.to_string() +
		"\n    Scale: " + scaler.to_string();
}

void Transform::translate(float x, float y, float z) {
	position.x += x;
	position.y += y;
	position.z += z;
}

void Transform::translateLocal(float const &x, float const &y, float const &z) {
	mtx::Vec3 translation = mtx::Vec3(x, y, z);
	//Logger::log("translate by: " + translation.to_string());
	translation.rotate(rotation);
	//Logger::log("rotated translate: " + translation.to_string());
	translate(translation);
}

void Transform::translateLocal(mtx::Vec3 translation) {
	translation.rotate(rotation);
	translate(translation);
}

void Transform::translate(mtx::Vec3 translation) {
	this->position += translation;
}

void Transform::rotate(float x, float y, float z) {
	rotation *= mtx::Quaternion().make_euler(x,y,z);
}

void Transform::rotate(mtx::Vec3 axis, float angle) {
	rotation *= mtx::Quaternion().make_rotation(axis, angle);
}
void Transform::rotate(mtx::Quaternion q) {
	rotation = q;
}

void Transform::scale(float x, float y, float z) {
	scaler.x *= x;
	scaler.y *= y;
	scaler.z *= z;
}

void Transform::scale(mtx::Vec3 scale) {
	scaler *= scale;
}

void Transform::scale(float amt) {
	scaler *= amt;
}

mtx::Vec3 Transform::getForwardVec() {
	return rotation.getForwardVec();
}

mtx::Vec3 Transform::getForwardFlatVec() {
	mtx::Vec3 forward = rotation.getForwardVec();
	forward.y = 0;
	forward.normalize();
	return forward;
}

mtx::Vec3 Transform::getUpVec() {
	return rotation.getUpVec();
}

mtx::Vec3 Transform::getRightVec() {
	return rotation.getRightVec();
}

/**
 * Decomposes a transformation matrix into this object's position, scaler, and rotation
 */
void Transform::setFromMatrix(mtx::Matrix4 matrix) {
	//position
	position.x = matrix.array[3];
	position.y = matrix.array[7];
	position.z = matrix.array[11];
	matrix.array[3] = 0;
	matrix.array[7] = 0;
	matrix.array[11] = 0;

	//scale
	scaler.x = mtx::Vec3(matrix.array[0], matrix.array[4], matrix.array[8]).magnitude();
	scaler.y = mtx::Vec3(matrix.array[1], matrix.array[5], matrix.array[9]).magnitude();
	scaler.z = mtx::Vec3(matrix.array[2], matrix.array[6], matrix.array[10]).magnitude();

	//rotation
	assert(scaler.x != 0);
	matrix.array[0] /= scaler.x;
	matrix.array[4] /= scaler.x;
	matrix.array[8] /= scaler.x;
	assert(scaler.y != 0);
	matrix.array[1] /= scaler.y;
	matrix.array[5] /= scaler.y;
	matrix.array[9] /= scaler.y;
	assert(scaler.z != 0);
	matrix.array[2] /= scaler.z;
	matrix.array[6] /= scaler.z;
	matrix.array[10] /= scaler.z;

	rotation.setFromMatrix(matrix);
}

void Transform::makeViewMatrix() {
	Logger::log(toString());
	global_transformation_matrix.make_translation(position);
	global_transformation_matrix.rotate(rotation);
}

void Transform::updateLocalTransformationMatrix() {
	local_transformation_matrix.make_scale(scaler);
	local_transformation_matrix.rotate(rotation);
	local_transformation_matrix.translate(position);
}

void Transform::updateGlobalTransformationMatrix() {
	
	if (containers.at(0)->parent != nullptr) {
		Transform* parent_transform;
		parent_transform = containers.at(0)->parent->getTransformComponent();
		global_transformation_matrix = parent_transform->global_transformation_matrix * local_transformation_matrix;
		//global_transformation_matrix = local_transformation_matrix;
		//DEBUG START
		if (Game::get().is_debug_mode) {
			Logger::log("    Name: " + containers.at(0)->directory_and_name);
			Logger::log("    Parent: " + containers.at(0)->parent->directory_and_name);
			Logger::log("    " + toString());
			Logger::log("    Local Matrix: \n " + local_transformation_matrix.to_string(1));
		}
		//DEBUG END
	} else {
		global_transformation_matrix = local_transformation_matrix;
		//DEBUG START
		if(Game::get().is_debug_mode) {
		Logger::log("    Name: " + containers.at(0)->directory_and_name);
		Logger::log("    Parent: none");
		Logger::log("    " + toString());
		Logger::log("    Local Matrix: \n " + local_transformation_matrix.to_string(1));
		}
		//DEBUG END
	}
	//DEBUG START
	if(Game::get().is_debug_mode) Logger::log("    Global Matrix: \n" + global_transformation_matrix.to_string(2));
	//DEBUG END
}

void Transform::update() {
	updateLocalTransformationMatrix();
	updateGlobalTransformationMatrix();
}