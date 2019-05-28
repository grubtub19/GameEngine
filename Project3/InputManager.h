#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <string>
#include <vector>
#include <map>
#include "Mtx.h"

struct KeyStatus {
	KeyStatus();
	KeyStatus(int key);
	int key;
	bool is_down;
	bool just_pressed;
};

class InputManager {
public:
	InputManager();
	~InputManager();

	std::map<std::string, KeyStatus> keys;
	mtx::Vec2 mouse_movement;

	void initKeys();
	void registerNewControl(std::string name, int key);
	void resetJustPressed();

	static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
	static void cursor_pos_callback(GLFWwindow* window, double xpos, double ypos);
	static void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
private:
	static bool instantiated;
	bool first_input;
	mtx::Vec2 current_mouse_position;
	

};
