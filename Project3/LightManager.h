#pragma once
#include "GlobalAmbientLight.h"

class LightManager {
public:
	LightManager();
	~LightManager();

	GlobalAmbientLight ambient_light;

	void init();
	void update();
	void shadowMapPass();

private:
	static bool instantiated;
};