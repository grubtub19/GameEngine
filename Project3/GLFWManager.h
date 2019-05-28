#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <string>

class Engine;

class GLFWManager {
public:
	GLFWManager();
	~GLFWManager();
	void start();
	void init();
	GLFWwindow* window;
	void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);

	void toggleFullScreen();
	bool is_fullscreen;
private:
	
	static bool instantiated;
};