#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <string>
#include <assert.h>
#include <vector>
#include <map>
#include "InputManager.h"
#include "Game.h"
#include "Camera.h"
#include "Logger.h"

bool InputManager::instantiated = false;

KeyStatus::KeyStatus() {}

KeyStatus::KeyStatus(int key) :
	key(key),
	is_down(false),
	just_pressed(false)
{

}

InputManager::InputManager() :
	current_mouse_position(mtx::Vec2(0, 0)),
	mouse_movement(mtx::Vec2(0, 0)),
	first_input(true),
	keys(std::map<std::string, KeyStatus>())
{
	assert(!instantiated);
	instantiated = true;
	initKeys();
	Logger::log("InputManager Loaded");
}

InputManager::~InputManager()
{
	Logger::log("InputManager Destruction");
}

void InputManager::initKeys() {
	keys.insert(std::make_pair(std::string("forward"), KeyStatus(GLFW_KEY_W)));
	keys.insert(std::make_pair(std::string("backward"), KeyStatus(GLFW_KEY_S)));
	keys.insert(std::make_pair(std::string("left"), KeyStatus(GLFW_KEY_A)));
	keys.insert(std::make_pair(std::string("right"), KeyStatus(GLFW_KEY_D)));
	keys.insert(std::make_pair(std::string("up"), KeyStatus(GLFW_KEY_SPACE)));
	keys.insert(std::make_pair(std::string("down"), KeyStatus(GLFW_KEY_LEFT_SHIFT)));
	keys.insert(std::make_pair(std::string("roll_left"), KeyStatus(GLFW_KEY_Q)));
	keys.insert(std::make_pair(std::string("roll_right"), KeyStatus(GLFW_KEY_E)));
	keys.insert(std::make_pair(std::string("animation_1"), KeyStatus(GLFW_KEY_1)));
	keys.insert(std::make_pair(std::string("animation_2"), KeyStatus(GLFW_KEY_2)));
	keys.insert(std::make_pair(std::string("animation_3"), KeyStatus(GLFW_KEY_3)));
	keys.insert(std::make_pair(std::string("animation_4"), KeyStatus(GLFW_KEY_4)));
	keys.insert(std::make_pair(std::string("animation_5"), KeyStatus(GLFW_KEY_5)));
	keys.insert(std::make_pair(std::string("animation_6"), KeyStatus(GLFW_KEY_6)));
	keys.insert(std::make_pair(std::string("animation_7"), KeyStatus(GLFW_KEY_7)));
	keys.insert(std::make_pair(std::string("animation_8"), KeyStatus(GLFW_KEY_8)));
	keys.insert(std::make_pair(std::string("animation_9"), KeyStatus(GLFW_KEY_9)));
	keys.insert(std::make_pair(std::string("animation_repeat_toggle"), KeyStatus(GLFW_KEY_GRAVE_ACCENT)));
	keys.insert(std::make_pair(std::string("light_forward"), KeyStatus(GLFW_KEY_UP)));
	keys.insert(std::make_pair(std::string("light_backward"), KeyStatus(GLFW_KEY_DOWN)));
	keys.insert(std::make_pair(std::string("light_left"), KeyStatus(GLFW_KEY_LEFT)));
	keys.insert(std::make_pair(std::string("light_right"), KeyStatus(GLFW_KEY_RIGHT)));
	keys.insert(std::make_pair(std::string("light_up"), KeyStatus(GLFW_KEY_COMMA)));
	keys.insert(std::make_pair(std::string("light_down"), KeyStatus(GLFW_KEY_PERIOD)));
	keys.insert(std::make_pair(std::string("toggle_fullscreen"), KeyStatus(GLFW_KEY_F)));
}
/**
 * Resets all just_pressed for the KeyStatus's in the InputManager to false
 */
void InputManager::resetJustPressed() {
	for (auto &[name, key_status] : Game::get().input_manager.keys) {
		key_status.just_pressed = false;
	}
	mouse_movement.x = 0;
	mouse_movement.y = 0;
}

/**
 * GLFW callback function for keyboard presses
 * Updates the KeyStatus's
 */
void InputManager::key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
	for (auto &[name, key_status] : Game::get().input_manager.keys) {
		if (key == key_status.key) {
			if (action == GLFW_PRESS) {
				key_status.is_down = true;
				key_status.just_pressed = true;
			}
			else if (action == GLFW_RELEASE) {
				key_status.is_down = false;
			}
		}
	}
}

/**
 * GLFW callback function for cursor position
 * Rotates the camera
 */
void InputManager::cursor_pos_callback(GLFWwindow* window, double xpos, double ypos) {

	if (!Game::get().input_manager.first_input) {
		Game::get().input_manager.mouse_movement.x = (float) (Game::get().input_manager.current_mouse_position.x - xpos);
		Game::get().input_manager.mouse_movement.y = (float) (Game::get().input_manager.current_mouse_position.y - ypos);
	} else {
		Game::get().input_manager.first_input = false;
	}

	Game::get().input_manager.current_mouse_position.x = (float) xpos;
	Game::get().input_manager.current_mouse_position.y = (float) ypos;
}

/**GLFW callback function for cursor button presses
 */
void InputManager::mouse_button_callback(GLFWwindow* window, int button, int action, int mods) {

}