#pragma once
#include "GLFWManager.h"
#include "Engine.h"
#include "LightManager.h"
#include "InputManager.h"
#include "TimeManager.h"
#include "DataManager.h"
#include "SceneManager.h"

class Game {
public:
	static Game& get();
	const bool is_debug_mode;
	void start();

	GLFWManager glfw;
	Engine engine;
	InputManager input_manager;
	TimeManager time_manager;
	LightManager light_manager;
	DataManager data_manager;
	SceneManager scene_manager;

private:
	static Game game;
	Game();	
};