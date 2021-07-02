#include "Light.h"

Light::Light() {

}

Light::Light(float ambient[4], float specular[4], float diffuse[4], float const& constant_att, float const& linear_att, float const& quad_att, float const& shininess) :
	ambient{ ambient[0], ambient[1], ambient[2], ambient[3] },
	specular{ specular[0], specular[1], specular[2], specular[3] },
	diffuse{ diffuse[0], diffuse[1], diffuse[2], diffuse[3] },
	constant_att(constant_att),
	linear_att(linear_att),
	quad_att(quad_att),
	shininess(shininess)
{
	
}

Light::~Light() {

}