#include <iostream>
#include <fstream>
#include <cstdlib>
#include <string_view>
#include <string>

#include "GLAD/glad.h"
#include "GLFW/glfw3.h"

#include "version.h"

namespace { 
	struct RGBA {
		GLfloat r;
		GLfloat g;
		GLfloat b;
		GLfloat a;
	};

	void logger(std::ofstream& logs_, const std::string_view& logs_file_name_, std::string_view& log_info_);

	void poll_events(GLFWwindow* window_);

	void framebuffer_size(GLFWwindow* window_, int width_, int height_);

	void append_title(std::string& window_title_);
}

int main() {
	std::ofstream logs;
	std::string_view logs_file_name("logs");
	int width = 2048;
	int height = 1024;
	std::string window_title = "g00dboyo ";
	const int opengl_version_major = 4;
	const int opengl_version_minor = 5;
	RGBA background_rgba{ 0.3f, 0.4f, 0.3f, 1.0f };

	int is_glfw_init = glfwInit();
	if (!is_glfw_init) {
		std::string_view log_info("GLFW dosn't initialized");

		logger(logs, logs_file_name, log_info);

		return EXIT_FAILURE;
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, opengl_version_major);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, opengl_version_minor);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#if defined __APPLE__  || defined __MACH__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GLFW_TRUE);
#endif // __APPLE__ || __MACH__

	append_title(window_title);

	GLFWwindow* window = glfwCreateWindow(width, height, window_title.data(), nullptr, nullptr);
	if (!window) {
		std::string_view log_info("GLFW window dosn't created");

		logger(logs, logs_file_name, log_info);

		glfwTerminate();

		return EXIT_FAILURE;
	}

	glfwMakeContextCurrent(window);

	GLADloadfunc loader = reinterpret_cast<GLADloadfunc>(glfwGetProcAddress);
	int is_glad_init = gladLoadGL(loader);

	if (!is_glad_init) {
		std::string_view log_info("GLAD dosn't initialized");

		logger(logs, logs_file_name, log_info);

		glfwTerminate();

		return EXIT_FAILURE;
	}
	
	glfwSetFramebufferSizeCallback(window, framebuffer_size);

	glClearColor(background_rgba.r, background_rgba.g, background_rgba.b, background_rgba.a);

	while (!glfwWindowShouldClose(window)) {
		glClear(GL_COLOR_BUFFER_BIT); // GL_DEPTH_BUFFER_BIT    GL_STENCIL_BUFFER_BIT

		glfwSwapBuffers(window);
		
		poll_events(window);
	}

	glfwTerminate();

	return EXIT_SUCCESS;
}

namespace {
	void logger(std::ofstream& logs_, const std::string_view& logs_file_name_, std::string_view& log_info_) {
		logs_.open(logs_file_name_.data(), std::ios::binary | std::ios::app | std::ios::out);

		if (logs_.is_open()) {
			logs_ << log_info_ << '\n';

			logs_.close();
		}
		else {
			std::cerr << log_info_ << '\n';
		}
	}

	void poll_events(GLFWwindow* window_) {
		int key_status = glfwGetKey(window_, GLFW_KEY_ESCAPE);
		if (key_status == GLFW_PRESS) {
			glfwSetWindowShouldClose(window_, GLFW_TRUE);
		}

		glfwPollEvents();
	}

	void framebuffer_size(GLFWwindow* window_, int width_, int height_) {
		const GLint x = 0;
		const GLint y = 0;

		glViewport(x, y, width_, height_);
	}

	void append_title(std::string& window_title_) {
#if defined(_WIN32) || defined(_WIN64)
		window_title_ = window_title_ + "Windows OS";
#elif defined(__APPLE__) || defined(__MACH__)
		window_title_ = window_title_ + "Mac OS";
#elif defined(linux) || defined(__linux) || defined(__linux__)
		window_title_ = window_title_ + "Linux OS";
#elif defined(unix) || defined(__unix) || defined(__unix__)
		window_title_ = window_title_ + "Unix OS";
#else
		window_title_ = window_title_ + "Unknown OS";
#endif // _WIN32 || _WIN64 OTHERS


#ifdef PROJ_VERSION_DISPL
		window_title_ = window_title_ + "  ver: " + std::to_string(VERSION_MAJOR) + '.' + std::to_string(VERSION_MINOR) + '.' + std::to_string(VERSION_PATCH);
#endif // PROJ_VERSION_DISPL
	}
}