#pragma once
#include <string>
#include <assimp/scene.h>

namespace mtx {
	
	float to_radians(float const &radians);

	class Quaternion;

	class Vec2 {
	public:

		float x, y;

		Vec2();
		Vec2(float x, float y);

		std::string to_string() const;

		Vec2 operator+(Vec2 const &right) const;
		Vec2 operator+(float const &right) const;

		Vec2& operator+=(Vec2 const &right);
		Vec2& operator+=(float const &right);

		Vec2 operator-(Vec2 const &right) const;
		Vec2 operator-(float const &right) const;

		Vec2& operator-=(Vec2 const &right);
		Vec2& operator-=(float const &right);

		Vec2 operator*(float const &right) const;
		Vec2& operator*=(float const &right);

		Vec2 operator/(float const &right) const;
		Vec2& operator/=(float const &right);

		bool operator==(Vec2 const &right) const;

		bool operator!=(Vec2 const &right) const;

		float* data_ptr();
	};

	class Vec3 {
	public:

		float x, y, z;

		Vec3();
		Vec3(float x, float y, float z);
		Vec3(const Vec3 &vector);
		Vec3(const aiVector3D& vector);

		std::string to_string() const;

		Vec3 operator-() const;

		Vec3 operator+(Vec3 const &right) const;
		Vec3 operator+(float const &right) const;

		Vec3& operator+=(Vec3 const &right);
		Vec3& operator+=(float const &right);

		Vec3 operator-(Vec3 const &right) const;
		Vec3 operator-(float const &right) const;

		Vec3& operator-=(Vec3 const &right);
		Vec3& operator-=(float const &right);

		Vec3 operator*(float const &right) const;
		Vec3 operator*(Vec3 const &right) const;
		Vec3& operator*=(float const &right);
		Vec3& operator*=(Vec3 const &right);

		Vec3 operator/(float const &right) const;
		Vec3& operator/=(float const &right);

		bool operator==(Vec3 const &right) const;

		bool operator!=(Vec3 const &right) const;
		
		float magnitude();

		float magnitude_squared();

		Vec3& normalize();
		Vec3 normal();

		float dot(Vec3 const &right) const;

		Vec3 cross(Vec3 const &right) const;

		Vec3 lerp(Vec3 const &right, const float time);

		Vec3 project_onto(Vec3 const &right);

		Vec3 perpendicular(Vec3 const &right);

		Vec3& rotate(Quaternion &quat);

		float* data_ptr();
	};
	
	class Matrix4 {
	public:
		float array[16];

		Matrix4();

		Matrix4(bool const &identity);

		Matrix4(float const &num);

		Matrix4(float a1, float a2, float a3, float a4, float b1, float b2, float b3, float b4, float c1, float c2, float c3, float c4, float d1, float d2, float d3, float d4);

		Matrix4(Matrix4 const &original);

		Matrix4(aiMatrix4x4 ai_matrix);

		std::string to_string();
		std::string to_string(int num_tabs);

		Matrix4& make_identity();

		Matrix4& make_translation(float const &x, float const &y, float const &z);
		Matrix4& make_translation(Vec3 const &direction);

		Matrix4& translate(float const &x, float const &y, float const &z);
		Matrix4& translate(Vec3 const &direction);

		Matrix4& make_scale(float const &x, float const &y, float const &z);
		Matrix4& make_scale(Vec3 const &scale);
		Matrix4& scale(float const &x, float const &y, float const &z);
		Matrix4& scale(Vec3 const &scale);
		
		Matrix4 inverse();
		Matrix4& invert();

		Matrix4& transpose();

		Matrix4& rotate(Quaternion const &quat);

		Matrix4& make_rotation(Quaternion const &quat);

		Matrix4 perspective(float const &fov, float const &aspect, float const &z_near, float const &z_far);

		Matrix4 look_at(const mtx::Vec3 &eye, const mtx::Vec3 &at, const mtx::Vec3 &up);
		Matrix4 look_at(mtx::Vec3 const &eye, mtx::Vec3 const &forward);

		float& operator[](int const &i);
		float operator[](int const &i) const;

		Matrix4& operator=(Matrix4 const &right);

		Matrix4 operator*(float const &right) const;
		Vec3 operator*(Vec3 const &right) const ;
		Matrix4 operator*(Matrix4 const &right)const ;

		Matrix4& operator*=(float const &right);
		Matrix4& operator*=(Matrix4 const &right);

		Matrix4& concatenate(Matrix4 const &right);
	};

	class Quaternion {
	public:
		Vec3 vector;
		float w;

		Quaternion();

		Quaternion(float const &x, float const &y, float const &z, float const &w);

		Quaternion(Vec3 const &vec, float const &w);
		Quaternion(Vec3 const &vec);

		Quaternion(Quaternion const &copy);
		Quaternion(aiQuaternion const &quat);
		
		void setFromMatrix(mtx::Matrix4 const &matrix);

		float sign(float x);
		float norm(float a, float b, float c, float d);

		float& get_x();
		float& get_y();
		float& get_z();

		float get_x() const;
		float get_y() const;
		float get_z() const;

		std::string to_string() const;

		Quaternion& operator=(Quaternion const &right);

		Quaternion operator+(Quaternion const &right);

		Quaternion& operator+=(Quaternion const &right);

		Quaternion operator-(Quaternion const &right);

		Quaternion& operator-=(Quaternion const &right);

		Quaternion operator*(float const &right) const;
		Quaternion operator*(Quaternion const &right) const;

		Quaternion& operator*=(float const &right);
		Quaternion& operator*=(Quaternion const &right);

		Quaternion& concatenate(Quaternion const &right);

		float dot(const Quaternion& right) const;

		float norm() const;
		float normSquared() const;

		Quaternion& scale(float const &num);

		Quaternion& normalize();
		Quaternion normal();

		Quaternion conjugate() const;

		Quaternion inverse() const;
		Quaternion& invert();

		Quaternion slerp(const Quaternion& right, float t) const;

		Matrix4 matrix() const;
		Quaternion& make_euler(float yaw, float pitch, float roll);
		Quaternion& make_rotation(Vec3 axis, float angle);
		Quaternion& rotate(Vec3 axis, float angle);
		Quaternion& rotate(Quaternion const &r);
		Vec3 getRightVec();
		Vec3 getForwardVec();
		Vec3 getUpVec();
	};
};