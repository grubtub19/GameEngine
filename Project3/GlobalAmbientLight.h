#pragma once
#include "data.h"

class GlobalAmbientLight {
public:
	GlobalAmbientLight();
	GlobalAmbientLight(float r, float g, float b, float a);
	~GlobalAmbientLight();
	RGBA rgba;

private:
	static bool instantiated;
};