#pragma once
#include "Mtx.h"
#include "Component.h"

class Transform : public Component {
public:
	Transform();
	Transform(float x, float y, float z);
	Transform(mtx::Vec3 const& transform);
	~Transform();

	mtx::Vec3 position;
	mtx::Quaternion rotation;
	mtx::Vec3 scaler;
	mtx::Matrix4 global_transformation_matrix;	
	mtx::Matrix4 local_transformation_matrix;

	std::string toString();

	void translate(float x, float y, float z);
	void translate(mtx::Vec3 translation);
	void translateLocal(float const& x, float const& y, float const& z);
	void translateLocal(mtx::Vec3 translation);

	void rotate(float x, float y, float z);
	void rotate(mtx::Vec3 axis, float angle);
	void rotate(mtx::Quaternion q);

	void scale(float x, float y, float z);
	void scale(mtx::Vec3 scale);
	void scale(float amt);

	mtx::Vec3 getForwardVec();
	mtx::Vec3 getForwardFlatVec();
	mtx::Vec3 getUpVec();
	mtx::Vec3 getRightVec();

	void setFromMatrix(mtx::Matrix4 matrix);
	
private:
	void updateLocalTransformationMatrix();
	void updateGlobalTransformationMatrix();
	void makeViewMatrix();

public:
	void update();

};