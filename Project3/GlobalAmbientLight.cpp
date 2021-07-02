#include <assert.h>
#include "GlobalAmbientLight.h"

bool GlobalAmbientLight::instantiated = false;

GlobalAmbientLight::GlobalAmbientLight() :
	rgba(RGBA(0, 0, 0, 0))
{
	assert(!instantiated);
	instantiated = true;
}

GlobalAmbientLight::GlobalAmbientLight(float r, float g, float b, float a) :
	rgba(RGBA(r, g, b, a))
{
	assert(!instantiated);
	instantiated = true;
}

GlobalAmbientLight::~GlobalAmbientLight() { }
