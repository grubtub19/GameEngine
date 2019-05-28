#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <string>
#include <iostream>
#include <assert.h>
#include "GLFWManager.h"
#include "Game.h"
#include "Logger.h"
#include "Engine.h"
#include "InputManager.h"
#define WINDOW_WIDTH 1280
#define WINDOW_HEIGHT 720

#define glCheckError() Logger::glCheckError_(__FILE__, __LINE__) 

bool GLFWManager::instantiated = false;

GLFWManager::GLFWManager() :
	window(nullptr),
	is_fullscreen(false)
{
	assert(!instantiated);
	instantiated = true;
}

GLFWManager::~GLFWManager() {
	Logger::log("Destroy GLFW window");
	glfwDestroyWindow(window);
	glfwTerminate();
	Logger::log("GLFWManager Destruction");
}

static void error_callback(int error, const char* description)
{
	fprintf(stderr, "Error: %s\n", description);
}

void APIENTRY glDebugOutput(GLenum source,
	GLenum type,
	GLuint id,
	GLenum severity,
	GLsizei length,
	const GLchar *message,
	const void *userParam)
{
	// ignore non-significant error/warning codes
	if (id == 131169 || id == 131185 || id == 131218 || id == 131204) return;

	std::cout << "---------------" << std::endl;
	std::cout << "Debug message (" << id << "): " << message << std::endl;

	switch (source)
	{
	case GL_DEBUG_SOURCE_API:             std::cout << "Source: API"; break;
	case GL_DEBUG_SOURCE_WINDOW_SYSTEM:   std::cout << "Source: Window System"; break;
	case GL_DEBUG_SOURCE_SHADER_COMPILER: std::cout << "Source: Shader Compiler"; break;
	case GL_DEBUG_SOURCE_THIRD_PARTY:     std::cout << "Source: Third Party"; break;
	case GL_DEBUG_SOURCE_APPLICATION:     std::cout << "Source: Application"; break;
	case GL_DEBUG_SOURCE_OTHER:           std::cout << "Source: Other"; break;
	} std::cout << std::endl;

	switch (type)
	{
	case GL_DEBUG_TYPE_ERROR:               std::cout << "Type: Error"; break;
	case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR: std::cout << "Type: Deprecated Behaviour"; break;
	case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:  std::cout << "Type: Undefined Behaviour"; break;
	case GL_DEBUG_TYPE_PORTABILITY:         std::cout << "Type: Portability"; break;
	case GL_DEBUG_TYPE_PERFORMANCE:         std::cout << "Type: Performance"; break;
	case GL_DEBUG_TYPE_MARKER:              std::cout << "Type: Marker"; break;
	case GL_DEBUG_TYPE_PUSH_GROUP:          std::cout << "Type: Push Group"; break;
	case GL_DEBUG_TYPE_POP_GROUP:           std::cout << "Type: Pop Group"; break;
	case GL_DEBUG_TYPE_OTHER:               std::cout << "Type: Other"; break;
	} std::cout << std::endl;

	switch (severity)
	{
	case GL_DEBUG_SEVERITY_HIGH:         std::cout << "Severity: high"; break;
	case GL_DEBUG_SEVERITY_MEDIUM:       std::cout << "Severity: medium"; break;
	case GL_DEBUG_SEVERITY_LOW:          std::cout << "Severity: low"; break;
	case GL_DEBUG_SEVERITY_NOTIFICATION: std::cout << "Severity: notification"; break;
	} std::cout << std::endl;
	std::cout << std::endl;
}

void GLFWManager::init() {

	Logger::log("Initializing GLFW");

	/* Initialize the library */
	if (!glfwInit()) {
		Logger::log("GLFW failed to initialize");
		exit(EXIT_FAILURE);
	}

	Logger::log("Loading Version 4.3");
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

	Logger::log("Loading Core Profile");
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	//Logger::log("Enabling Debug");
	//glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);

	Logger::log("Creating Window");
	/* Create a windowed mode window and its OpenGL context */
	window = glfwCreateWindow(1280, 720, "Why are you reading this Mailler?", NULL, NULL);
	glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);



	if (!window)
	{
		Logger::log("Failed to create window");
		glfwTerminate();
		exit(EXIT_FAILURE);
	}

	//glfwSetErrorCallback(error_callback);

	/* Make the window's context current */
	glfwMakeContextCurrent(window);

	Logger::log("Initializing GLAD");
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) /* There was an error initilizing GLAD */
	{
		Logger::log("Error! Failed to initlize OpenGL (GLAD)");
		exit(EXIT_FAILURE);
	}

	const GLubyte* vendor = glGetString(GL_VENDOR);
	const GLubyte* renderer = glGetString(GL_RENDERER);

	Logger::log(std::string((const char*)vendor));
	Logger::log(std::string((const char*)renderer));

	//GLFW setup
	glfwSetKeyCallback(window, InputManager::key_callback);
	glfwSetCursorPosCallback(window, InputManager::cursor_pos_callback);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwSwapInterval(1);

	GLint flags; glGetIntegerv(GL_CONTEXT_FLAGS, &flags);
	if (flags & GL_CONTEXT_FLAG_DEBUG_BIT)
	{
		glEnable(GL_DEBUG_OUTPUT);
		glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
		glDebugMessageCallback(glDebugOutput, nullptr); // segfault on this line
		glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr, GL_TRUE);
	}

	//Opengl calls
	glEnable(GL_DEPTH_TEST);
}

void GLFWManager::toggleFullScreen() {
	if (is_fullscreen) {
		glfwSetWindowMonitor(window, NULL, 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, 60);
	}
}

void GLFWManager::start() {
	/* Loop until the user closes the window */
	while (!glfwWindowShouldClose(window))
	{
		if (Game::get().input_manager.keys.at("toggle_fullscreen").just_pressed) {
			toggleFullScreen();
		}
		//Logger::log("start");
		Game::get().engine.update();
		Game::get().engine.draw();
		//Logger::log("end");
	}
	Logger::log("GLFW window should close");
	glfwTerminate();
}