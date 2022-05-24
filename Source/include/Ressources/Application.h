#pragma once
#pragma region Includes

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

#define stb_image_implementation
#include <stb_image.h>

// Core
#include "../../include/Core/Debug/Log.h"
#include "../../include/Core/Debug/Assertion.h"

// Ressource
#include "../../include/Ressources/Model.h"
#include "../../include/Ressources/RessourcesManager.h"
#include "../../include/Ressources/Shader.h"

// LowRenderer
#include "../../include/LowRenderer/Camera.h"
#include "../../include/LowRenderer/Mesh.h"

// Maths
#include "Vec3/Vec3.h"

#pragma endregion

#pragma region USING NAMESPACE
using namespace LowRenderer;
#pragma endregion

class application
{
private:
	GLFWwindow* window_;

	const int scr_width_;
    const int scr_height_;

	mat4 mvp_;

	int init_window(const char* window_name, int window_width, int window_height);
	int init_glad();

	std::vector<Mesh> meshes_;

public:
	float delta_time_;
	float last_time_;
	float current_time_;

	application(const char* window_name, int window_width, int window_height);

	void delta_time();

	void run();

	static void framebuffer_size_callback(GLFWwindow* window, int width, int height);

	static void process_input(GLFWwindow* window, Camera& camera, float delta_time);
	static void keyboard_callback(GLFWwindow* window, Camera& camera, float delta_time);
	static void mouse_callback(GLFWwindow* window, Camera& camera);

	static void APIENTRY gl_debug_output(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* user_param);
};
