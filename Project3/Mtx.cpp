#include "Mtx.h"
#include <string>
#include <sstream>
#include <iomanip>
#include <cmath>
#include <assimp/scene.h>
#include "Logger.h"

# define M_PI           3.14159265358979323846

float mtx::to_radians(float const &radians) {
	return radians / 360 * (float)M_PI * 2;
}

mtx::Vec2::Vec2() : x(0), y(0) { }

mtx::Vec2::Vec2(float x, float y) : x(x), y(y) { }

std::string mtx::Vec2::to_string() const {
	std::string rtn;
	rtn.reserve(50);
	std::string str = std::to_string(x);
	str.erase(str.find_last_not_of('0') + 1, std::string::npos);
	str.erase(str.find_last_not_of('.') + 1, std::string::npos);
	rtn += str + "\n";
	str = std::to_string(y);
	str.erase(str.find_last_not_of('0') + 1, std::string::npos);
	str.erase(str.find_last_not_of('.') + 1, std::string::npos);
	rtn += str + "\n";
	return rtn;
}

/*----------------------
		 ADDITION
  ----------------------*/

  /**
   *	Vec2 const + Vec2
   *	NOTE: Follows Vector + Point rules for w
   */
mtx::Vec2 mtx::Vec2::operator+(Vec2 const &right) const {
	return Vec2(x + right.x, y + right.y);
}

/**
 * Vec2 const + float
 */
mtx::Vec2 mtx::Vec2::operator+(float const &right) const {
	return Vec2(x + right, y + right);
}

/**
 *	Vec2 += Vec2
 *	NOTE: w stays the same
 */
mtx::Vec2& mtx::Vec2::operator+=(Vec2 const &right) {
	x += right.x;
	y += right.y;
	return *this;
}

/**
 *	Vec2 += float
 */
mtx::Vec2& mtx::Vec2::operator+=(float const &right) {
	x += right;
	y += right;
	return *this;
}

/*----------------------
		SUBTRACTION
  ----------------------*/

  /**
   *	Vec2 const - Vec2
   *	NOTE: Follows Vector - Point rules for w
   */
mtx::Vec2 mtx::Vec2::operator-(Vec2 const &right) const {
	return Vec2(x - right.x, y - right.y);
}

/**
 *	Vect4 - float
 */
mtx::Vec2 mtx::Vec2::operator-(float const &right) const {
	return Vec2(x - right, y - right);
}

/**
 *	Vec2 -= Vec2
 *	NOTE: w stays the same
 */
mtx::Vec2& mtx::Vec2::operator-=(Vec2 const &right) {
	x -= right.x;
	y -= right.y;
	return *this;
}

/**
 *	Vec2 -= float
 */
mtx::Vec2& mtx::Vec2::operator-=(float const &right) {
	x -= right;
	y -= right;
	return *this;
}


/*----------------------
	  MULTIPLICATION
  ----------------------*/

  // Vec2 * scalar

/**
 *
 */
mtx::Vec2 mtx::Vec2::operator*(float const &right) const {
	return Vec2(x * right, y * right);
}

/**
 *	Vec2 *= float
 */
mtx::Vec2& mtx::Vec2::operator*=(float const &right) {
	x *= right;
	y *= right;
	return *this;
}

/*----------------------
		 DIVISION
  ----------------------*/

  /**
   *	Vec2 / float
   */
mtx::Vec2 mtx::Vec2::operator/(float const &right) const {
	return Vec2(x / right, y / right);
}

/**
 *	Vec2 /= float
 */
mtx::Vec2& mtx::Vec2::operator/=(float const &right) {
	x /= right;
	y /= right;
	return *this;
}

/*----------------------
		 EQUALITY
  ----------------------*/

/**
 *	Vec2 == Vec2
 *	NOTE: Vector != Point
 */
bool mtx::Vec2::operator==(Vec2 const &right) const {
	return x == right.x && y == right.y;
}

/**
 *	Vec2 != Vec2
 *	NOTE: Vector != Point
 */
bool mtx::Vec2::operator!=(Vec2 const &right) const {
	return x != right.x || y != right.y;
}

/**
 *	Returns pointer to the start of the Vec2. Useful for OpenGL calls
 */
float* mtx::Vec2::data_ptr() {
	return &x;
}

mtx::Vec3::Vec3() :x(0), y(0), z(0) {}

mtx::Vec3::Vec3(float x, float y, float z) : x(x), y(y), z(z) {}

mtx::Vec3::Vec3(const Vec3 &vector) : x(vector.x), y(vector.y), z(vector.z) {}

mtx::Vec3::Vec3(const aiVector3D& vector) : x(vector.x), y(vector.y), z(vector.z) {}

std::string mtx::Vec3::to_string() const {
	std::string rtn;
	rtn.reserve(50);
	std::string str = std::to_string(x);
	str.erase(str.find_last_not_of('0') + 1, std::string::npos);
	str.erase(str.find_last_not_of('.') + 1, std::string::npos);
	rtn += "[ x: " + str + ", ";
	str = std::to_string(y);
	str.erase(str.find_last_not_of('0') + 1, std::string::npos);
	str.erase(str.find_last_not_of('.') + 1, std::string::npos);
	rtn += "y: " + str + ", ";
	str = std::to_string(z);
	str.erase(str.find_last_not_of('0') + 1, std::string::npos);
	str.erase(str.find_last_not_of('.') + 1, std::string::npos);
	rtn += "z: " + str + " ]";
	return rtn;
}

mtx::Vec3 mtx::Vec3::operator-() const {
	return Vec3(-x, -y, -z);
}

/*----------------------
		 ADDITION
  ----------------------*/

/**
 *	Vec3 const + Vec3
 *	NOTE: Follows Vector + Point rules for w
 */
mtx::Vec3 mtx::Vec3::operator+(Vec3 const &right) const {
	return Vec3(x + right.x, y + right.y, z + right.z);
}

/**
 * Vec3 const + float
 */
mtx::Vec3 mtx::Vec3::operator+(float const &right) const {
	return Vec3(x + right, y + right, z + right);
}

/**
 *	Vec3 += Vec3
 *	NOTE: w stays the same
 */
mtx::Vec3& mtx::Vec3::operator+=(Vec3 const &right) {
	x += right.x;
	y += right.y;
	z += right.z;
	return *this;
}


/**
 *	Vec3 += float
 */
mtx::Vec3& mtx::Vec3::operator+=(float const &right) {
	x += right;
	y += right;
	z += right;
	return *this;
}

/*----------------------
		SUBTRACTION
  ----------------------*/

/**
 *	Vec3 const - Vec3
 *	NOTE: Follows Vector - Point rules for w
 */
mtx::Vec3 mtx::Vec3::operator-(Vec3 const &right) const {
	return Vec3(x - right.x, y - right.y, z - right.z);
}

/**
 *	Vect4 - float
 */
mtx::Vec3 mtx::Vec3::operator-(float const &right) const {
	return Vec3(x - right, y - right, z - right);
}

/**
 *	Vec3 -= Vec3
 *	NOTE: w stays the same
 */
mtx::Vec3& mtx::Vec3::operator-=(Vec3 const &right) {
	x -= right.x;
	y -= right.y;
	z -= right.z;
	return *this;
}

/**
 *	Vec3 -= float
 */
mtx::Vec3& mtx::Vec3::operator-=(float const &right) {
	x -= right;
	y -= right;
	z -= right;
	return *this;
}


/*----------------------
	  MULTIPLICATION
  ----------------------*/

  // Vec3 * scalar

/**
 *
 */
mtx::Vec3 mtx::Vec3::operator*(float const &right) const {
	return Vec3(x * right, y * right, z * right);
}

mtx::Vec3 mtx::Vec3::operator*(Vec3 const &right) const {
	return Vec3(x * right.x, y * right.y, z * right.z);
}

/**
 *	Vec3 *= float
 */
mtx::Vec3& mtx::Vec3::operator*=(float const &right) {
	x *= right;
	y *= right;
	z *= right;
	return *this;
}

/**
 *	Vec3 *= Vec3
 */
mtx::Vec3& mtx::Vec3::operator*=(Vec3 const &right) {
	x *= right.x;
	y *= right.y;
	z *= right.z;
	return *this;
}

/*----------------------
		 DIVISION
  ----------------------*/

/**
 *	Vec3 / float
 */
mtx::Vec3 mtx::Vec3::operator/(float const &right) const {
	return Vec3(x / right, y / right, z / right);
}

/**
 *	Vec3 /= float
 */
mtx::Vec3& mtx::Vec3::operator/=(float const &right) {
	x /= right;
	y /= right;
	z /= right;
	return *this;
}

/*----------------------
		 EQUALITY
  ----------------------*/

/**
 *	Vec3 == Vec3
 *	NOTE: Vector != Point
 */
bool mtx::Vec3::operator==(Vec3 const &right) const {
	return x == right.x && y == right.y && z == right.z;
}

/**
 *	Vec3 != Vec3
 *	NOTE: Vector != Point
 */
bool mtx::Vec3::operator!=(Vec3 const &right) const {
	return x != right.x || y != right.y || z != right.z;
}

/**
 *	|Vec3|
 */
float mtx::Vec3::magnitude() {
	return std::sqrt(x * x + y * y + z * z);
}

/**
 *	|Vec3|^2
 */
float mtx::Vec3::magnitude_squared() {
	return x * x + y * y + z * z;
}

/**
 *	Normalizes the Vec3
 */
mtx::Vec3& mtx::Vec3::normalize() {
	float mag = magnitude();
	if (mag > 0) {
		*this /= mag;
	}
	return (*this);
}

/**
 *	Returns a normalized version of a the Vec3
 */
mtx::Vec3 mtx::Vec3::normal() {
	float mag = magnitude();
	if (mag > 0) {
		return Vec3(x / mag, y / mag, z / mag);
	}
	else {
		return Vec3(*this);
	}
}

/**
 *	Returns the dot product of two Vec3
 */
float mtx::Vec3::dot(Vec3 const &right) const {
	return x * right.x + y * right.y + z * right.z;
}

/**
 *	Returns the cross product of two Vec3's
 *	Always returns a Vector, not a Point (w = 0)
 */
mtx::Vec3 mtx::Vec3::cross(Vec3 const &right) const {
	return Vec3(
		y * right.z - z * right.y,
		z * right.x - x * right.z,
		x * right.y - y * right.x);
}

/**
 *	Linear Interpolation starting at the current Vec3 (time = 0) and moving toward the "right" Vec3 (time = 1)
 */
mtx::Vec3 mtx::Vec3::lerp(Vec3 const &right, float const time) {
	return  (*this) * (1 - time) + right * time;
}

/**
 *	Projection of this Vec3 onto another Vec3
 *	Returns a Vector, not a Point (w = 0)
 */
mtx::Vec3 mtx::Vec3::project_onto(Vec3 const &right) {
	return right * (dot(right) / magnitude_squared());
}

/**
 * Rotates Vec3 by a Quaternion
 * NOTE: Normalizes the quaternion's original value during
 */
mtx::Vec3& mtx::Vec3::rotate(Quaternion &quat) {
	quat.normalize();
	*this = (quat * Quaternion(*this) * quat.conjugate()).vector;
	return (*this);
}

/**
 *	Returns pointer to the start of the Vec3. Useful for OpenGL calls
 */
float* mtx::Vec3::data_ptr() {
	return &x;
}

//-------------------------------
//         MATRIX TIME
//-------------------------------
/*
[  0  1  2  3 ]
[  4  5  6  7 ]
[  8  9 10 11 ]
[ 12 13 14 15 ]
*/
/**
 * Default Constructor
 */
mtx::Matrix4::Matrix4() : array{ 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f } {}

mtx::Matrix4::Matrix4(bool const &identity) : array{ 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f } {

}

/**
 * Constructor that fills every entry with the parameter
 */
mtx::Matrix4::Matrix4(float const &num) : array{ num, num, num, num, num, num, num, num, num, num, num, num, num, num, num, num } {}

/**
 * Constructor of 16 floats
 */
mtx::Matrix4::Matrix4(float a1, float a2, float a3, float a4, float b1, float b2, float b3, float b4, float c1, float c2, float c3, float c4, float d1, float d2, float d3, float d4) :
	array{ a1, a2, a3, a4, b1, b2, b3, b4, c1, c2, c3, c4, d1, d2, d3, d4 } {}

mtx::Matrix4::Matrix4(Matrix4 const &original) : array() {
	for (int i = 0; i < 16; i++) {
		array[i] = original[i];
	}
}

mtx::Matrix4::Matrix4(aiMatrix4x4 ai_matrix) {
	array[0] = ai_matrix.a1;
	array[1] = ai_matrix.a2;
	array[2] = ai_matrix.a3;
	array[3] = ai_matrix.a4;
	array[4] = ai_matrix.b1;
	array[5] = ai_matrix.b2;
	array[6] = ai_matrix.b3;
	array[7] = ai_matrix.b4;
	array[8] = ai_matrix.c1;
	array[9] = ai_matrix.c2;
	array[10] = ai_matrix.c3;
	array[11] = ai_matrix.c4;
	array[12] = ai_matrix.d1;
	array[13] = ai_matrix.d2;
	array[14] = ai_matrix.d3;
	array[15] = ai_matrix.d4;
}

/**
 * Formats the Matrix4 into a 4x4 string
 */
std::string mtx::Matrix4::to_string() {
	std::ostringstream rtn;
	std::string numbers[16];
	int length = 0;
	int length_r = 0;
	for (int i = 0; i < 16; i++) {
		numbers[i] = std::to_string(array[i]);
		numbers[i].erase(numbers[i].find_last_not_of('0') + 1, std::string::npos);
		numbers[i].erase(numbers[i].find_last_not_of('.') + 1, std::string::npos);
		if ((i + 1) % 4 != 0 && numbers[i].length() > length) {
			length = numbers[i].length();
		}
		else if (numbers[i].length() > length_r) {
			length_r = numbers[i].length();
		}
	}
	for (int i = 0; i < 16; i++) {
		if (i % 4 == 0) {
			rtn << "[ ";
		}
		if ((i + 1) % 4 != 0) {
			rtn << std::setw(length + 1) << std::left << numbers[i];
		}
		else {
			rtn << std::setw(length_r + 1) << std::left << numbers[i];
			rtn << "]\n";
		}
	}
	return rtn.str();
}

std::string mtx::Matrix4::to_string(int num_tabs) {
	std::ostringstream rtn;
	std::string numbers[16];
	int length = 0;
	int length_r = 0;
	for (int i = 0; i < 16; i++) {
		numbers[i] = std::to_string(array[i]);
		numbers[i].erase(numbers[i].find_last_not_of('0') + 1, std::string::npos);
		numbers[i].erase(numbers[i].find_last_not_of('.') + 1, std::string::npos);
		if ((i + 1) % 4 != 0 && numbers[i].length() > length) {
			length = numbers[i].length();
		}
		else if (numbers[i].length() > length_r) {
			length_r = numbers[i].length();
		}
	}
	for (int i = 0; i < 16; i++) {
		if (i % 4 == 0) {
			for (int j = 0; j < num_tabs; j++) {
				rtn << "	";
			}
			rtn << "[ ";
		}
		
		if ((i + 1) % 4 != 0) {
			rtn << std::setw(length + 1) << std::left << numbers[i];
		} else {
			rtn << std::setw(length_r + 1) << std::left << numbers[i];
			rtn << "]\n";
		}
	}
	return rtn.str();
}

/**
 * Sets the matrix to the identity matrix
 */
mtx::Matrix4& mtx::Matrix4::make_identity() {
	array[0] = 1.0f;
	array[1] = 0.0f;
	array[2] = 0.0f;
	array[3] = 0.0f;

	array[4] = 0.0f;
	array[5] = 1.0f;
	array[6] = 0.0f;
	array[7] = 0.0f;

	array[8] = 0.0f;
	array[9] = 0.0f;
	array[10] = 1.0f;
	array[11] = 0.0f;

	array[12] = 0.0f;
	array[13] = 0.0f;
	array[14] = 0.0f;
	array[15] = 1.0f;
	return (*this);
}

/**
 * Sets the matrix to the translation matrix given the translation floats x, y, and z
 */
mtx::Matrix4& mtx::Matrix4::make_translation(float const &x, float const &y, float const &z) {
	array[0] = 1.0f;	array[1] = 0.0f;	array[2] = 0.0f;	array[3] = x;
	array[4] = 0.0f;	array[5] = 1.0f;	array[6] = 0.0f;	array[7] = y;
	array[8] = 0.0f;	array[9] = 0.0f;	array[10] = 1.0f;	array[11] = z;
	array[12] = 0.0f;	array[13] = 0.0f;	array[14] = 0.0f;	array[15] = 1.0f;
	return (*this);
}

/**
 * Sets the matrix to the translation matrix given the translation Vec3
 */
mtx::Matrix4& mtx::Matrix4::make_translation(Vec3 const &direction) {
	array[0] = 1.0f;	array[1] = 0.0f;	array[2] = 0.0f;	array[3] = direction.x;
	array[4] = 0.0f;	array[5] = 1.0f;	array[6] = 0.0f;	array[7] = direction.y;
	array[8] = 0.0f;	array[9] = 0.0f;	array[10] = 1.0f;	array[11] = direction.z;
	array[12] = 0.0f;	array[13] = 0.0f;	array[14] = 0.0f;	array[15] = 1.0f;
	return (*this);
}

/**
 * Translates the Matrix4 by x, y, and z
 * @returns Matrix4& (*this)
 */
mtx::Matrix4& mtx::Matrix4::translate(float const &x, float const &y, float const &z) {
	array[3] += x;
	array[7] += y;
	array[11] += z;
	return (*this);
}

/**
 * Translates the Matrix4 by the Vec3
 * @returns (*this)
 */
mtx::Matrix4& mtx::Matrix4::translate(Vec3 const &direction) {
	array[3] += direction.x;
	array[7] += direction.y;
	array[11] += direction.z;
	return (*this);
}

/**
 * Assigns the Matrix4 to a scale matrix
 */
mtx::Matrix4& mtx::Matrix4::make_scale(float const &x, float const &y, float const &z) {
	array[0] = x;		array[1] = 0.0f;	array[2] = 0.0f;	array[3] = 0.0f;
	array[4] = 0.0f;	array[5] = y;		array[6] = 0.0f;	array[7] = 0.0f;
	array[8] = 0.0f;	array[9] = 0.0f;	array[10] = z;		array[11] = 0.0f;
	array[12] = 0.0f;	array[13] = 0.0f;	array[14] = 0.0f;	array[15] = 1.0f;
	return (*this);
}

mtx::Matrix4& mtx::Matrix4::make_scale(mtx::Vec3 const &scale) {
	array[0] = scale.x;	array[1] = 0.0f;	array[2] = 0.0f;	array[3] = 0.0f;
	array[4] = 0.0f;	array[5] = scale.y;	array[6] = 0.0f;	array[7] = 0.0f;
	array[8] = 0.0f;	array[9] = 0.0f;	array[10] = scale.z;array[11] = 0.0f;
	array[12] = 0.0f;	array[13] = 0.0f;	array[14] = 0.0f;	array[15] = 1.0f;
	return (*this);
}

/**
 * Left hand multiplication of a scale matrix to the Matrix4
 */
mtx::Matrix4& mtx::Matrix4::scale(float const &x, float const &y, float const &z) {
	array[0] *= x;
	array[1] *= x;
	array[2] *= x;
	array[4] *= y;
	array[5] *= y;
	array[6] *= y;
	array[8] *= z;
	array[9] *= z;
	array[10] *= z;
	return (*this);
}

/**
 * Left hand multiplication of a scale matrix to the Matrix4
 */
mtx::Matrix4& mtx::Matrix4::scale(mtx::Vec3 const &scale) {
	array[0] *= scale.x;
	array[1] *= scale.x;
	array[2] *= scale.x;
	array[4] *= scale.y;
	array[5] *= scale.y;
	array[6] *= scale.y;
	array[8] *= scale.z;
	array[9] *= scale.z;
	array[10] *= scale.z;
	return (*this);
}

/**
 * Inverse of a Matrix
 * @returns Matrix4 result
 */
mtx::Matrix4 mtx::Matrix4::inverse() {
	//
	// Inversion by Cramer's rule.  Code taken from an Intel publication
	//
	mtx::Matrix4 result;
	float tmp[12]; /* temp array for pairs */
	float src[16]; /* array of transpose source matrix */
	float det; /* determinant */
	/* transpose matrix */
	for (int i = 0; i < 4; i++)
	{
		src[i + 0] = array[4 * i];
		src[i + 4] = array[4 * i + 1];
		src[i + 8] = array[4 * i + 2];
		src[i + 12] = array[4 * i + 3];
	}
	/* calculate pairs for first 8 elements (cofactors) */
	tmp[0] = src[10] * src[15];
	tmp[1] = src[11] * src[14];
	tmp[2] = src[9] * src[15];
	tmp[3] = src[11] * src[13];
	tmp[4] = src[9] * src[14];
	tmp[5] = src[10] * src[13];
	tmp[6] = src[8] * src[15];
	tmp[7] = src[11] * src[12];
	tmp[8] = src[8] * src[14];
	tmp[9] = src[10] * src[12];
	tmp[10] = src[8] * src[13];
	tmp[11] = src[9] * src[12];
	/* calculate first 8 elements (cofactors) */
	result[0] = tmp[0] * src[5] + tmp[3] * src[6] + tmp[4] * src[7];
	result[0] -= tmp[1] * src[5] + tmp[2] * src[6] + tmp[5] * src[7];
	result[1] = tmp[1] * src[4] + tmp[6] * src[6] + tmp[9] * src[7];
	result[1] -= tmp[0] * src[4] + tmp[7] * src[6] + tmp[8] * src[7];
	result[2] = tmp[2] * src[4] + tmp[7] * src[5] + tmp[10] * src[7];
	result[2] -= tmp[3] * src[4] + tmp[6] * src[5] + tmp[11] * src[7];
	result[3] = tmp[5] * src[4] + tmp[8] * src[5] + tmp[11] * src[6];
	result[3] -= tmp[4] * src[4] + tmp[9] * src[5] + tmp[10] * src[6];
	result[4] = tmp[1] * src[1] + tmp[2] * src[2] + tmp[5] * src[3];
	result[4] -= tmp[0] * src[1] + tmp[3] * src[2] + tmp[4] * src[3];
	result[5] = tmp[0] * src[0] + tmp[7] * src[2] + tmp[8] * src[3];
	result[5] -= tmp[1] * src[0] + tmp[6] * src[2] + tmp[9] * src[3];
	result[6] = tmp[3] * src[0] + tmp[6] * src[1] + tmp[11] * src[3];
	result[6] -= tmp[2] * src[0] + tmp[7] * src[1] + tmp[10] * src[3];
	result[7] = tmp[4] * src[0] + tmp[9] * src[1] + tmp[10] * src[2];
	result[7] -= tmp[5] * src[0] + tmp[8] * src[1] + tmp[11] * src[2];
	/* calculate pairs for second 8 elements (cofactors) */
	tmp[0] = src[2] * src[7];
	tmp[1] = src[3] * src[6];
	tmp[2] = src[1] * src[7];
	tmp[3] = src[3] * src[5];
	tmp[4] = src[1] * src[6];
	tmp[5] = src[2] * src[5];

	tmp[6] = src[0] * src[7];
	tmp[7] = src[3] * src[4];
	tmp[8] = src[0] * src[6];
	tmp[9] = src[2] * src[4];
	tmp[10] = src[0] * src[5];
	tmp[11] = src[1] * src[4];
	/* calculate second 8 elements (cofactors) */
	result[8] = tmp[0] * src[13] + tmp[3] * src[14] + tmp[4] * src[15];
	result[8] -= tmp[1] * src[13] + tmp[2] * src[14] + tmp[5] * src[15];
	result[9] = tmp[1] * src[12] + tmp[6] * src[14] + tmp[9] * src[15];
	result[9] -= tmp[0] * src[12] + tmp[7] * src[14] + tmp[8] * src[15];
	result[10] = tmp[2] * src[12] + tmp[7] * src[13] + tmp[10] * src[15];
	result[10] -= tmp[3] * src[12] + tmp[6] * src[13] + tmp[11] * src[15];
	result[11] = tmp[5] * src[12] + tmp[8] * src[13] + tmp[11] * src[14];
	result[11] -= tmp[4] * src[12] + tmp[9] * src[13] + tmp[10] * src[14];
	result[12] = tmp[2] * src[10] + tmp[5] * src[11] + tmp[1] * src[9];
	result[12] -= tmp[4] * src[11] + tmp[0] * src[9] + tmp[3] * src[10];
	result[13] = tmp[8] * src[11] + tmp[0] * src[8] + tmp[7] * src[10];
	result[13] -= tmp[6] * src[10] + tmp[9] * src[11] + tmp[1] * src[8];
	result[14] = tmp[6] * src[9] + tmp[11] * src[11] + tmp[3] * src[8];
	result[14] -= tmp[10] * src[11] + tmp[2] * src[8] + tmp[7] * src[9];
	result[15] = tmp[10] * src[10] + tmp[4] * src[8] + tmp[9] * src[9];
	result[15] -= tmp[8] * src[9] + tmp[11] * src[10] + tmp[5] * src[8];
	/* calculate determinant */
	det = src[0] * result[0] + src[1] * result[1] + src[2] * result[2] + src[3] * result[3];
	/* calculate matrix inverse */
	det = 1.0f / det;

	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			result[4 * i + j] *= det;
		}
	}
	return result;
}

/**
 * Inverse of a Matrix
 * @returns Matrix4& (*this)
 */
mtx::Matrix4& mtx::Matrix4::invert() {
	//
// Inversion by Cramer's rule.  Code taken from an Intel publication
//
	float tmp[12]; /* temp array for pairs */
	float src[16]; /* array of transpose source matrix */
	float det; /* determinant */
	/* transpose matrix */
	for (int i = 0; i < 4; i++)
	{
		src[i + 0] = array[4 * i];
		src[i + 4] = array[4 * i + 1];
		src[i + 8] = array[4 * i + 2];
		src[i + 12] = array[4 * i + 3];
	}
	/* calculate pairs for first 8 elements (cofactors) */
	tmp[0] = src[10] * src[15];
	tmp[1] = src[11] * src[14];
	tmp[2] = src[9] * src[15];
	tmp[3] = src[11] * src[13];
	tmp[4] = src[9] * src[14];
	tmp[5] = src[10] * src[13];
	tmp[6] = src[8] * src[15];
	tmp[7] = src[11] * src[12];
	tmp[8] = src[8] * src[14];
	tmp[9] = src[10] * src[12];
	tmp[10] = src[8] * src[13];
	tmp[11] = src[9] * src[12];
	/* calculate first 8 elements (cofactors) */
	array[0] = tmp[0] * src[5] + tmp[3] * src[6] + tmp[4] * src[7];
	array[0] -= tmp[1] * src[5] + tmp[2] * src[6] + tmp[5] * src[7];
	array[1] = tmp[1] * src[4] + tmp[6] * src[6] + tmp[9] * src[7];
	array[1] -= tmp[0] * src[4] + tmp[7] * src[6] + tmp[8] * src[7];
	array[2] = tmp[2] * src[4] + tmp[7] * src[5] + tmp[10] * src[7];
	array[2] -= tmp[3] * src[4] + tmp[6] * src[5] + tmp[11] * src[7];
	array[3] = tmp[5] * src[4] + tmp[8] * src[5] + tmp[11] * src[6];
	array[3] -= tmp[4] * src[4] + tmp[9] * src[5] + tmp[10] * src[6];
	array[4] = tmp[1] * src[1] + tmp[2] * src[2] + tmp[5] * src[3];
	array[4] -= tmp[0] * src[1] + tmp[3] * src[2] + tmp[4] * src[3];
	array[5] = tmp[0] * src[0] + tmp[7] * src[2] + tmp[8] * src[3];
	array[5] -= tmp[1] * src[0] + tmp[6] * src[2] + tmp[9] * src[3];
	array[6] = tmp[3] * src[0] + tmp[6] * src[1] + tmp[11] * src[3];
	array[6] -= tmp[2] * src[0] + tmp[7] * src[1] + tmp[10] * src[3];
	array[7] = tmp[4] * src[0] + tmp[9] * src[1] + tmp[10] * src[2];
	array[7] -= tmp[5] * src[0] + tmp[8] * src[1] + tmp[11] * src[2];
	/* calculate pairs for second 8 elements (cofactors) */
	tmp[0] = src[2] * src[7];
	tmp[1] = src[3] * src[6];
	tmp[2] = src[1] * src[7];
	tmp[3] = src[3] * src[5];
	tmp[4] = src[1] * src[6];
	tmp[5] = src[2] * src[5];

	tmp[6] = src[0] * src[7];
	tmp[7] = src[3] * src[4];
	tmp[8] = src[0] * src[6];
	tmp[9] = src[2] * src[4];
	tmp[10] = src[0] * src[5];
	tmp[11] = src[1] * src[4];
	/* calculate second 8 elements (cofactors) */
	array[8] = tmp[0] * src[13] + tmp[3] * src[14] + tmp[4] * src[15];
	array[8] -= tmp[1] * src[13] + tmp[2] * src[14] + tmp[5] * src[15];
	array[9] = tmp[1] * src[12] + tmp[6] * src[14] + tmp[9] * src[15];
	array[9] -= tmp[0] * src[12] + tmp[7] * src[14] + tmp[8] * src[15];
	array[10] = tmp[2] * src[12] + tmp[7] * src[13] + tmp[10] * src[15];
	array[10] -= tmp[3] * src[12] + tmp[6] * src[13] + tmp[11] * src[15];
	array[11] = tmp[5] * src[12] + tmp[8] * src[13] + tmp[11] * src[14];
	array[11] -= tmp[4] * src[12] + tmp[9] * src[13] + tmp[10] * src[14];
	array[12] = tmp[2] * src[10] + tmp[5] * src[11] + tmp[1] * src[9];
	array[12] -= tmp[4] * src[11] + tmp[0] * src[9] + tmp[3] * src[10];
	array[13] = tmp[8] * src[11] + tmp[0] * src[8] + tmp[7] * src[10];
	array[13] -= tmp[6] * src[10] + tmp[9] * src[11] + tmp[1] * src[8];
	array[14] = tmp[6] * src[9] + tmp[11] * src[11] + tmp[3] * src[8];
	array[14] -= tmp[10] * src[11] + tmp[2] * src[8] + tmp[7] * src[9];
	array[15] = tmp[10] * src[10] + tmp[4] * src[8] + tmp[9] * src[9];
	array[15] -= tmp[8] * src[9] + tmp[11] * src[10] + tmp[5] * src[8];
	/* calculate determinant */
	det = src[0] * array[0] + src[1] * array[1] + src[2] * array[2] + src[3] * array[3];
	/* calculate matrix inverse */
	det = 1.0f / det;

	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			array[4 * i + j] *= det;
		}
	}
	return (*this);
}

/**
 * Transposes the Matrix4
 */
mtx::Matrix4& mtx::Matrix4::transpose() {
	float temp;
	temp = array[1];
	array[1] = array[4];
	array[4] = temp;

	temp = array[2];
	array[2] = array[8];
	array[8] = temp;

	temp = array[3];
	array[3] = array[12];
	array[12] = temp;

	temp = array[6];
	array[6] = array[9];
	array[9] = temp;

	temp = array[7];
	array[7] = array[13];
	array[13] = temp;

	temp = array[11];
	array[11] = array[14];
	array[14] = temp;
	return (*this);
}

mtx::Matrix4& mtx::Matrix4::rotate(Quaternion const &quat) {
	Matrix4 rtn = Matrix4(true);
	rtn[0] = 1 - 2 * quat.get_y() * quat.get_y() - 2 * quat.get_z() * quat.get_z();
	rtn[1] = 2 * quat.get_x() * quat.get_y() - 2 * quat.get_z() * quat.w;
	rtn[2] = 2 * quat.get_x() * quat.get_z() + 2 * quat.get_y() * quat.w;
	rtn[4] = 2 * quat.get_x() * quat.get_y() + 2 * quat.get_z() * quat.w;
	rtn[5] = 1 - 2 * quat.get_x() * quat.get_x() - 2 * quat.get_z() * quat.get_z();
	rtn[6] = 2 * quat.get_y() * quat.get_z() - 2 * quat.get_x() * quat.w;
	rtn[8] = 2 * quat.get_x() * quat.get_z() - 2 * quat.get_y() * quat.w;
	rtn[9] = 2 * quat.get_y() * quat.get_z() + 2 * quat.get_x() * quat.w;
	rtn[10] = 1 - 2 * quat.get_x() * quat.get_x() - 2 * quat.get_y() * quat.get_y();
	concatenate(rtn);
	return (*this);
}

mtx::Matrix4& mtx::Matrix4::make_rotation(Quaternion const &quat) {
	array[0] = 1 - 2 * quat.get_y() * quat.get_y() - 2 * quat.get_z() * quat.get_z();
	array[1] = 2 * quat.get_x() * quat.get_y() - 2 * quat.get_z() * quat.w;
	array[2] = 2 * quat.get_x() * quat.get_z() + 2 * quat.get_y() * quat.w;
	array[3] = 0;
	array[4] = 2 * quat.get_x() * quat.get_y() + 2 * quat.get_z() * quat.w;
	array[5] = 1 - 2 * quat.get_x() * quat.get_x() - 2 * quat.get_z() * quat.get_z();
	array[6] = 2 * quat.get_y() * quat.get_z() - 2 * quat.get_x() * quat.w;
	array[7] = 0;
	array[8] = 2 * quat.get_x() * quat.get_z() - 2 * quat.get_y() * quat.w;
	array[9] = 2 * quat.get_y() * quat.get_z() + 2 * quat.get_x() * quat.w;
	array[10] = 1 - 2 * quat.get_x() * quat.get_x() - 2 * quat.get_y() * quat.get_y();
	array[11] = 0;
	array[12] = 0;
	array[13] = 0;
	array[14] = 0;
	array[15] = 1;
	return (*this);
}

mtx::Matrix4 mtx::Matrix4::perspective(float const &fov, float const &aspect, float const &z_near, float const &z_far) {
	float width = 1.0f / tanf(fov / 2.0f), height = aspect / tanf(fov / 2.0f);

	Matrix4 rtn;
	rtn[0] = width;
	rtn[4] = 0.0f;
	rtn[8] = 0.0f;
	rtn[12] = 0.0f;

	rtn[1] = 0.0f;
	rtn[5] = height;
	rtn[9] = 0.0f;
	rtn[13] = 0.0f;

	rtn[2] = 0.0f;
	rtn[6] = 0.0f;
	rtn[10] = z_far / (z_near - z_far);
	rtn[14] = z_far * z_near / (z_near - z_far);

	rtn[3] = 0.0f;
	rtn[7] = 0.0f;
	rtn[11] = -1.0f;
	rtn[15] = 0.0f;
	return rtn;
}

mtx::Matrix4 mtx::Matrix4::look_at(const mtx::Vec3 &eye, const mtx::Vec3 &at, const mtx::Vec3 &up)
{
	Vec3 x_axis, y_axis, z_axis;
	z_axis = (eye - at).normalize();
	x_axis = up.cross(z_axis).normalize();
	y_axis = z_axis.cross(x_axis).normalize();

	Matrix4 result;
	result[0] = x_axis.x;
	result[4] = x_axis.y;
	result[8] = x_axis.z;
	result[12] = -x_axis.dot(eye);

	result[1] = y_axis.x;
	result[5] = y_axis.y;
	result[9] = y_axis.z;
	result[13] = -y_axis.dot(eye);

	result[2] = z_axis.x;
	result[6] = z_axis.y;
	result[10] = z_axis.z;
	result[14] = -z_axis.dot(eye);

	result[3] = 0.0f;
	result[7] = 0.0f;
	result[11] = 0.0f;
	result[15] = 1.0f;
	return result;
}

mtx::Matrix4 mtx::Matrix4::look_at(mtx::Vec3 const &eye, mtx::Vec3 const &forward) {
	Vec3 backward = -forward;
	Vec3 right = Vec3(0.0f, 1.0f, 0.0f).cross(backward).normalize();
	Vec3 up = backward.cross(right);


	Matrix4 result;
	result[0] = right.x;
	result[4] = right.y;
	result[8] = right.z;
	result[12] = -right.dot(eye);

	result[1] = up.x;
	result[5] = up.y;
	result[9] = up.z;
	result[13] = -up.dot(eye);

	result[2] = -forward.x;
	result[6] = -forward.y;
	result[10] = -forward.z;
	result[14] = forward.dot(eye);

	result[3] = 0.0f;
	result[7] = 0.0f;
	result[11] = 0.0f;
	result[15] = 1.0f;
	return result;
}

/**
 * Operator Overload []
 */
float& mtx::Matrix4::operator[](int const &i) {
	return array[i];
}

/**
 * Operator Overload []
 */
float mtx::Matrix4::operator[](int const &i) const {
	return array[i];
}

/**
 * Assignment Operator
 * Copies values
 */
mtx::Matrix4& mtx::Matrix4::operator=(Matrix4 const &right) {
	for (int i = 0; i < 16; i++) {
		array[i] = right[i];
	}
	return (*this);
}

/**
 * Left Multiplies the Matrix4 by the a scale matrix with x, y, and z equal to right
 * @returns Matrix4 the product
 */
mtx::Matrix4 mtx::Matrix4::operator*(float const &right) const {
	Matrix4 rtn = Matrix4(*this);
	rtn[0] = array[0] * right;
	rtn[1] = array[1] * right;
	rtn[2] = array[2] * right;
	rtn[4] = array[4] * right;
	rtn[5] = array[5] * right;
	rtn[6] = array[6] * right;
	rtn[8] = array[8] * right;
	rtn[9] = array[9] * right;
	rtn[10] = array[10] * right;
	return rtn;
}

/**
 * Matrix4 * Vec3
  * @returns Vec3
 */
mtx::Vec3 mtx::Matrix4::operator*(mtx::Vec3 const &right) const {
	Vec3 result;
	result.x = right.x * array[0] + right.y * array[1] + right.z * array[2] + 1 * array[3];
	result.y = right.x * array[4] + right.y * array[5] + right.z * array[6] + 1 * array[7];
	result.z = right.x * array[8] + right.y * array[9] + right.z * array[10] + 1 * array[11];
	return result;
}

/**
 * Matrix Multiplication
 * @returns Matrix4 the product
 */
mtx::Matrix4 mtx::Matrix4::operator*(Matrix4 const &right) const {
	Matrix4 result;
	for (int i = 0; i < 4; i++)
	{
		for (int i2 = 0; i2 < 4; i2++)
		{
			float total = 0.0f;
			for (int i3 = 0; i3 < 4; i3++)
			{
				total += array[4 * i + i3] * right[4 * i3 + i2];
			}
			result[4 * i + i2] = total;
		}
	}
	return result;
}

/**
 * Left Multiplies the Matrix4 by the a scale matrix with x, y, and z equal to right
 * @returns Matrix4& (*this)
 */
mtx::Matrix4& mtx::Matrix4::operator*=(float const &right) {
	array[0] *= right;
	array[1] *= right;
	array[2] *= right;
	array[4] *= right;
	array[5] *= right;
	array[6] *= right;
	array[8] *= right;
	array[9] *= right;
	array[10] *= right;
	return (*this);
}

/**
 * Matrix Multiplication
 * @returns Matrix4& (*this)
 */
mtx::Matrix4& mtx::Matrix4::operator*=(const Matrix4 &right) {
	Matrix4 result;
	for (int i = 0; i < 4; i++)
	{
		for (int i2 = 0; i2 < 4; i2++)
		{
			float total = 0.0f;
			for (int i3 = 0; i3 < 4; i3++)
			{
				total += array[4 * i + i3] * right[4 * i3 + i2];
			}
			result[4 * i + i2] = total;
		}
	}
	*this = result;
	return (*this);
}

/**
 * Left Multiplication
 */
mtx::Matrix4& mtx::Matrix4::concatenate(Matrix4 const &right) {
	Matrix4 result;
	for (int i = 0; i < 4; i++)
	{
		for (int i2 = 0; i2 < 4; i2++)
		{
			float total = 0.0f;
			for (int i3 = 0; i3 < 4; i3++)
			{
				total += right[4 * i + i3] * array[4 * i3 + i2];
			}
			result[4 * i + i2] = total;
		}
	}
	*this = result;
	return (*this);
}

mtx::Quaternion::Quaternion() : vector(mtx::Vec3(0,0,0)), w(1) {}

mtx::Quaternion::Quaternion(float const &x, float const &y, float const &z, float const &w) : vector{ x, y, z }, w(w) {}

mtx::Quaternion::Quaternion(Vec3 const &vec, float const &w) : vector(vec), w(w) {}

mtx::Quaternion::Quaternion(Vec3 const &vec) : vector(vec), w(0) {}

mtx::Quaternion::Quaternion(Quaternion const &copy) : vector{ copy.get_x(), copy.get_y(), copy.get_z() }, w(copy.w) {}

mtx::Quaternion::Quaternion(aiQuaternion const& quat) : w(quat.w), vector(mtx::Vec3(quat.x, quat.y, quat.z)) {}

void mtx::Quaternion::setFromMatrix(Matrix4 const& matrix) {
	float r11 = matrix.array[0];
	float r12 = matrix.array[1];
	float r13 = matrix.array[2];
	float r21 = matrix.array[4];
	float r22 = matrix.array[5];
	float r23 = matrix.array[6];
	float r31 = matrix.array[8];
	float r32 = matrix.array[9];
	float r33 = matrix.array[10];
	w = (r11 + r22 + r33 + 1.0f) / 4.0f;
	vector.x = (r11 - r22 - r33 + 1.0f) / 4.0f;
	vector.y = (-r11 + r22 - r33 + 1.0f) / 4.0f;
	vector.z = (-r11 - r22 + r33 + 1.0f) / 4.0f;
	if (w < 0.0f) {
		w = 0.0f;
	}
	if (vector.x < 0.0f) {
		vector.x = 0.0f;
	}
	if (vector.y < 0.0f) {
		vector.y = 0.0f;
	}
	if (vector.z < 0.0f) {
		vector.z = 0.0f;
	}
	w = sqrt(w);
	vector.x = sqrt(vector.x);
	vector.y = sqrt(vector.y);
	vector.z = sqrt(vector.z);
	if (w >= vector.x && w >= vector.y && w >= vector.z) {
		w *= +1.0f;
		vector.x *= sign(r32 - r23);
		vector.y *= sign(r13 - r31);
		vector.z *= sign(r21 - r12);
	}
	else if (vector.x >= w && vector.x >= vector.y && vector.x >= vector.z) {
		w *= sign(r32 - r23);
		vector.x *= +1.0f;
		vector.y *= sign(r21 + r12);
		vector.z *= sign(r13 + r31);
	}
	else if (vector.y >= w && vector.y >= vector.x && vector.y >= vector.z) {
		w *= sign(r13 - r31);
		vector.x *= sign(r21 + r12);
		vector.y *= +1.0f;
		vector.z *= sign(r32 + r23);
	}
	else if (vector.z >= w && vector.z >= vector.x && vector.z >= vector.y) {
		w *= sign(r21 - r12);
		vector.x *= sign(r31 + r13);
		vector.y *= sign(r32 + r23);
		vector.z *= +1.0f;
	}
	else {
		printf("coding error\n");
	}
	float r = norm(w, vector.x, vector.y, vector.z);
	w /= r;
	vector.x /= r;
	vector.y /= r;
	vector.z /= r;
}

float mtx::Quaternion::sign(float x) {
	return (x >= 0.0f) ? +1.0f : -1.0f;
}

float mtx::Quaternion::norm(float a, float b, float c, float d) {
	return sqrt(a * a + b * b + c * c + d * d);
}

float& mtx::Quaternion::get_x() {
	return vector.x;
}

float& mtx::Quaternion::get_y() {
	return vector.y;
}

float& mtx::Quaternion::get_z() {
	return vector.z;
}

float mtx::Quaternion::get_x() const {
	return vector.x;
}

float mtx::Quaternion::get_y() const {
	return vector.y;
}

float mtx::Quaternion::get_z() const {
	return vector.z;
}

std::string mtx::Quaternion::to_string() const {
	std::string rtn;
	rtn.reserve(50);
	std::string str = std::to_string(w);
	str.erase(str.find_last_not_of('0') + 1, std::string::npos);
	str.erase(str.find_last_not_of('.') + 1, std::string::npos);
	rtn += "[ w: " + str + ", ";
	str = std::to_string(get_x());
	str.erase(str.find_last_not_of('0') + 1, std::string::npos);
	str.erase(str.find_last_not_of('.') + 1, std::string::npos);
	rtn += "x: " + str + ", ";
	str = std::to_string(get_y());
	str.erase(str.find_last_not_of('0') + 1, std::string::npos);
	str.erase(str.find_last_not_of('.') + 1, std::string::npos);
	rtn += "y: " + str + ", ";
	str = std::to_string(get_z());
	str.erase(str.find_last_not_of('0') + 1, std::string::npos);
	str.erase(str.find_last_not_of('.') + 1, std::string::npos);
	rtn += "z: " + str + " ]";
	return rtn;
}

mtx::Quaternion& mtx::Quaternion::operator=(Quaternion const &copy) {
	get_x() = copy.get_x();
	get_y() = copy.get_y();
	get_z() = copy.get_z();
	w = copy.w;
	return (*this);
}

mtx::Quaternion mtx::Quaternion::operator+(Quaternion const &right) {
	return Quaternion(get_x() + right.get_x(), get_y() + right.get_y(), get_z() + right.get_z(), w + right.w);
}

mtx::Quaternion& mtx::Quaternion::operator+=(Quaternion const &right) {
	get_x() += right.get_x();
	get_y() += right.get_y();
	get_z() += right.get_z();
	w += right.w;
	return (*this);
}

mtx::Quaternion mtx::Quaternion::operator-(Quaternion const &right) {
	return Quaternion(get_x() - right.get_x(), get_y() - right.get_y(), get_z() - right.get_z(), w - right.w);
}

mtx::Quaternion& mtx::Quaternion::operator-=(Quaternion const &right) {
	get_x() -= right.get_x();
	get_y() -= right.get_y();
	get_z() -= right.get_z();
	w -= right.w;
	return (*this);
}

mtx::Quaternion mtx::Quaternion::operator*(float const &right) const {
	return Quaternion(vector * right, w * right);
}

mtx::Quaternion mtx::Quaternion::operator*(Quaternion const &right) const {
	/*Logger::log("Quaternion multiplication result: " + Quaternion(
		get_x()*right.w + get_y()*right.get_z() - get_z()*right.get_y() + w * right.get_x(),
		-get_x()*right.get_z() + get_y()*right.w + get_z()*right.get_x() + w * right.get_y(),
		get_x()*right.get_y() - get_y()*right.get_x() + get_z()*right.w + w * right.get_z(),
		-get_x()*right.get_x() - get_y()*right.get_y() - get_z()*right.get_z() + w * right.w).to_string());*/
	return Quaternion(
		get_x()*right.w + get_y()*right.get_z() - get_z()*right.get_y() + w*right.get_x(),
		-get_x()*right.get_z() + get_y()*right.w + get_z()*right.get_x() + w*right.get_y(),
		get_x()*right.get_y() - get_y()*right.get_x() + get_z()*right.w + w*right.get_z(),
		-get_x()*right.get_x() - get_y()*right.get_y() - get_z()*right.get_z() + w*right.w);
}

mtx::Quaternion& mtx::Quaternion::operator*=(float const &right) {
	vector *= right;
	w *= right;
	return (*this);
}

mtx::Quaternion& mtx::Quaternion::operator*=(Quaternion const &right) {
	Vec3 vec = right.vector * w + vector * right.w + vector.cross(right.vector);
	w = w * right.w - vector.dot(right.vector);
	vector = vec;
	return (*this);
}

mtx::Quaternion& mtx::Quaternion::concatenate(Quaternion const &right) {
	(*this) = right * (*this);
	return (*this);
}

float mtx::Quaternion::dot(const Quaternion& right) const {
	return this->get_x()* right.get_x() + this->get_y() * right.get_y() + this->get_z() * right.get_z() + this->w * right.w;
}

float mtx::Quaternion::norm() const {
	return std::sqrt(get_x() * get_x() + get_y() * get_y() + get_z() * get_z() + w * w);
}

float mtx::Quaternion::normSquared() const {
	return (get_x() * get_x() + get_y() * get_y() + get_z() * get_z() + w * w);
}

mtx::Quaternion& mtx::Quaternion::scale(float const &num) {
	vector *= num;
	w *= num;
	return (*this);
}

mtx::Quaternion& mtx::Quaternion::normalize() {
	float norm = Quaternion::norm();
	vector /= norm;
	w /= norm;
	return (*this);
}

mtx::Quaternion mtx::Quaternion::normal() {

	float norm = Quaternion::norm();
	return Quaternion(vector /= norm, w /= norm);
}

mtx::Quaternion mtx::Quaternion::conjugate() const {
	return Quaternion(-vector, w);
}

mtx::Quaternion& mtx::Quaternion::invert() {
	float n = normSquared();
	*this = conjugate() * (1 / n);
	return (*this);
}

mtx::Quaternion mtx::Quaternion::inverse() const {
	float n = normSquared();
	return Quaternion(conjugate() * (1 / n));
}

mtx::Quaternion mtx::Quaternion::slerp(const Quaternion& right, float t) const {
	float w1, x1, y1, z1, w2, x2, y2, z2, w3, x3, y3, z3;
	float theta, mult1, mult2;

	w1 = this->w; x1 = this->vector.x; y1 = this->vector.y; z1 = this->vector.z;
	w2 = right.w; x2 = right.vector.x; y2 = right.vector.y; z2 = right.vector.z;

	// Reverse the sign of q2 if q1.q2 < 0.
	if (w1 * w2 + x1 * x2 + y1 * y2 + z1 * z2 < 0)
	{
		w2 = -w2; x2 = -x2; y2 = -y2; z2 = -z2;
	}

	theta = acos(w1 * w2 + x1 * x2 + y1 * y2 + z1 * z2);

	if (theta > 0.000001)
	{
		mult1 = sin((1 - t) * theta) / sin(theta);
		mult2 = sin(t * theta) / sin(theta);
	}

	// To avoid division by 0 and by very small numbers the approximation of sin(angle)
	// by angle is used when theta is small (0.000001 is chosen arbitrarily).
	else
	{
		mult1 = 1 - t;
		mult2 = t;
	}

	w3 = mult1 * w1 + mult2 * w2;
	x3 = mult1 * x1 + mult2 * x2;
	y3 = mult1 * y1 + mult2 * y2;
	z3 = mult1 * z1 + mult2 * z2;

	return Quaternion( x3, y3, z3, w3);
}

mtx::Matrix4 mtx::Quaternion::matrix() const {
	return Matrix4().make_rotation(*this);
}

mtx::Quaternion& mtx::Quaternion::make_euler(float yaw, float pitch, float roll) {
	yaw *= (float) (M_PI / 180.0f);
	pitch *= (float) (M_PI / 180.0f);
	float cy = std::cos(yaw * 0.5f);
	float sy = std::sin(yaw * 0.5f);
	float cp = std::cos(pitch * 0.5f);
	float sp = std::sin(pitch * 0.5f);
	float cr = std::cos(roll * 0.5f);
	float sr = std::sin(roll * 0.5f);

	w = cy * cp * cr + sy * sp * sr;
	get_x() = cy * cp * sr - sy * sp * cr;
	get_y() = sy * cp * sr + cy * sp * cr;
	get_z() = sy * cp * cr - cy * sp * sr;

	return *this;
}

mtx::Quaternion& mtx::Quaternion::make_rotation(Vec3 axis, float angle) {
	angle *= (M_PI / 180.0f);
	Vec3 norm_axis = axis.normal();
	w = std::cosf(angle / 2.0f);
	vector.x = norm_axis.x * std::sinf(angle / 2.0f);
	vector.y = norm_axis.y * std::sinf(angle / 2.0f);
	vector.z = norm_axis.z * std::sinf(angle / 2.0f);
	return *this;
}

mtx::Quaternion& mtx::Quaternion::rotate(Vec3 axis, float angle) {
	Quaternion r = Quaternion().make_rotation(axis, angle);
	*this = r * (*this) * r.invert();
	return *this;
}

mtx::Quaternion& mtx::Quaternion::rotate(Quaternion const &r) {
	*this = r * (*this) * r.inverse();
	return *this;
}

/**
 * Forward of vector that initially was looking down the -z axis rotated by the quaternion
 * @return Vec3 Forward vector
 */
mtx::Vec3 mtx::Quaternion::getForwardVec() {
	Vec3 forward = mtx::Vec3(0.0f, 0.0f, -1.0f);
	Logger::log("Rotation: " + to_string());
	forward.rotate(*this);
	//Logger::log("Forward Vector: " + forward.to_string());
	return forward;
}

/**
 * Right of vector that initially was looking down the -z axis rotated by the quaternion
 * @return Vec3 Right vector
 */
mtx::Vec3 mtx::Quaternion::getRightVec() {
	mtx::Vec3 right = mtx::Vec3(1.0f, 0.0f, 0.0f);
	right.rotate(*this);
	Logger::log("Right Vector: " + right.to_string());
	return right;
}

/**
 * Up of vector that initially was looking down the -z axis rotated by the quaternion
 * @return Vec3 Up vector
 */
mtx::Vec3 mtx::Quaternion::getUpVec() {
	mtx::Vec3 up = mtx::Vec3(0.0f, 1.0f, 0.0f);
	up.rotate(*this);
	Logger::log("Up Vector: " + up.to_string());
	return up;
}

