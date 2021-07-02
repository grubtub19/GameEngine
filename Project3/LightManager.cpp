#include <assert.h>
#include "LightManager.h"
#include "GlobalAmbientLight.h"
#include "Logger.h"

bool LightManager::instantiated = false;

LightManager::LightManager() :
	ambient_light(GlobalAmbientLight(0.5f, 0.4f, 0.4f, 1.0f))
{
	assert(!instantiated);
	instantiated = true;
	Logger::log("LightManager Loaded");
}

LightManager::~LightManager() 
{
	Logger::log("LightManager Destruction");
}

void LightManager::init() {

}

void LightManager::update() {

}

void LightManager::shadowMapPass() {

}
