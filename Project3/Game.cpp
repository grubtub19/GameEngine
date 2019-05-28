#include "Game.h"
#include "Logger.h"
#include "GLFWManager.h"
#include "Engine.h"
#include "LightManager.h"
#include "InputManager.h"
#include "TimeManager.h"
#include "DataManager.h"
#include "SceneManager.h"

Game Game::game = Game();

Game& Game::get() {
	return game;
}

Game::Game() : 
	is_debug_mode(false),
	glfw(GLFWManager()),
	engine(Engine()), 
	light_manager(LightManager()),
	input_manager(InputManager()),
	time_manager(TimeManager()),
	data_manager(DataManager()),
	scene_manager(SceneManager())
{
	
	Logger::log("Game Loaded");
}

void Game::start() {
	Logger::log("Game Initialized");
	glfw.init();
	engine.init();
	glfw.start();
}