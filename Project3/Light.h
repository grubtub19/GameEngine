#pragma once
#include "Component.h"
#include "Mtx.h"

class Transform;

class Light : public Component {
public:
	Light();
	Light(float ambient[4], float specular[4], float diffuse[4], float const& constant_att, float const& linear_att, float const& quad_att, float const& shininess);
	~Light();

	float ambient[4] = { 0.2f, 0.1f, 0.1f, 1.0f };
	float specular[4] = { 1.0f, 1.0f, 1.0f, 0.2f };
	float diffuse[4] = { 1.0f, 1.0f, 1.0f, 0.2f };
	float constant_att = 0.0;
	float linear_att = 0.001;
	float quad_att = 0.0;
	float shininess = 1.0f;
};