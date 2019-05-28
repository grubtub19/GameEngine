//#include <GLFW/glfw3.h>
#include <string>
#include <memory>
#include <assert.h>
#include "Game.h"
#include "Engine.h"
#include "Shader.h"
#include "Logger.h"
#include "GLFWManager.h"

bool Engine::instantiated = false;

Engine::Engine() :
	height(0), width(0)
{
	assert(!instantiated);
	instantiated = true;
	Logger::log("Initializing Constructed");
}

Engine::~Engine() {
	Logger::log("Engine Deconstructed");
}

//Initialize Opengl stuff
void Engine::init() {
	Game::get().scene_manager.init();
}

void Engine::update() {
	/* Poll for and process events */
	Game::get().input_manager.resetJustPressed();
	//Logger::log("start");
	glfwPollEvents();
	//Logger::log("        poll events finished");
	Game::get().time_manager.update();

	Game::get().scene_manager.update();
	//Logger::log("after update");
}

void Engine::draw() {	
	
	//Logger::log("before draw");
	/* Render here */
	glClearColor(0.21f, 0.56f, 0.82f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glfwGetFramebufferSize(Game::get().glfw.window, &width, &height);
	glViewport(0, 0, width, height);

	Game::get().scene_manager.draw();
	//Logger::log("after draw");
	/* Swap front and back buffers */
	glfwSwapBuffers(glfwGetCurrentContext());
	//Logger::log("after swap buffers");

}