#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <string>
#include <fstream>

#define glCheckError() Logger::glCheckError_(__FILE__, __LINE__) 

class Logger {
	public:
		
		static bool erased;
		static int bone_vertex_data_count;
		static void log(const std::string&);
		static const char * GetGLErrorStr(GLenum err);
		static GLenum glCheckError_(const char *file, int line);
		static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
};