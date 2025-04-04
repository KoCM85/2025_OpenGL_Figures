#include <cstdlib>
#include <string_view>
#include <string>
#include <array>
#include <vector>


#include "GLAD/glad.h"
#include "GLFW/glfw3.h"

#include "version.h"
#include "shader.h"
#include "gl_utility.h"
#include "shape.h"

namespace { 
	struct RGBA {
		GLfloat r;
		GLfloat g;
		GLfloat b;
		GLfloat a;
	};

	void poll_events(GLFWwindow* window_);

	void framebuffer_size(GLFWwindow* window_, int width_, int height_);

	void append_title(std::string& window_title_);
}

int main() {
	int width = 2048;
	int height = 1024;
	std::string window_title = "g00dboyo ";
	const int opengl_version_major = 4;
	const int opengl_version_minor = 5;
	RGBA background_rgba{ 0.3f, 0.4f, 0.3f, 1.0f };

	std::string_view point_attrib_name = "point";
	std::string_view color_attrib_name = "color";
	std::string_view time_attrib_name = "time";

	constexpr size_t program_size = 2;
	std::array<std::string_view, program_size> vertex_shader_files{ "1_staticObj_interpolation.vert" , "2_StaticObj.vert" };
	std::array<std::string_view, program_size> fragment_shader_files{ "1_Interpolation.frag", "2_dynamicInterpolation.frag" };
	std::vector<shader> vertex_shaders;
	std::vector<shader> fragment_shaders;
	std::vector<shader_program> programs;

	int is_glfw_init = glfwInit();
	if (!is_glfw_init) {
		std::string_view log_info("GLFW dosn't initialized");

		logger(log_info);

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

		logger(log_info);

		glfwTerminate();

		return EXIT_FAILURE;
	}

	glfwMakeContextCurrent(window);

	GLADloadfunc loader = reinterpret_cast<GLADloadfunc>(glfwGetProcAddress);
	int is_glad_init = gladLoadGL(loader);

	if (!is_glad_init) {
		std::string_view log_info("GLAD dosn't initialized");

		logger(log_info);

		glfwTerminate();

		return EXIT_FAILURE;
	}
	
	glfwSetFramebufferSizeCallback(window, framebuffer_size);
	
	vertex_shaders.reserve(vertex_shader_files.size());
	fragment_shaders.reserve(fragment_shader_files.size());

	for (auto&& vertex_shader_file : vertex_shader_files) {
		vertex_shaders.emplace_back(vertex_shader_file, GL_VERTEX_SHADER);
		vertex_shaders.back().configure_shader();
		GLint vertex_shader_status = vertex_shaders.back().shader_status(GL_COMPILE_STATUS);

		if (!vertex_shader_status) {
			std::string_view log_info("Vertex shader dosn't compiled");

			logger(log_info);

			glfwTerminate();

			return EXIT_FAILURE;
		}
	}

	for (auto&& fragment_shader_file : fragment_shader_files) {
		fragment_shaders.emplace_back(fragment_shader_file, GL_FRAGMENT_SHADER);
		fragment_shaders.back().configure_shader();
		GLint fragment_shader_status = fragment_shaders.back().shader_status(GL_COMPILE_STATUS);

		if (!fragment_shader_status) {
			std::string_view log_info("Fragment shader dosn't compiled");

			logger(log_info);

			glfwTerminate();

			return EXIT_FAILURE;
		}
	}

	programs.reserve(program_size);

	for (size_t index = 0; index < program_size; ++index) {
		programs.emplace_back();
		programs[index].attach_shader(vertex_shaders[index].get_id(), fragment_shaders[index].get_id());
		programs[index].link_program();
		GLint program_status = programs[index].program_status(GL_LINK_STATUS);

		if (!program_status) {
			std::string_view log_info("Shader program dosn't linked");

			logger(log_info);

			glfwTerminate();

			return EXIT_FAILURE;
		}

		programs[index].delete_shaders();
	}

	std::vector<shape::attrib_t> attribs_1{
		{point_attrib_name, 3, GL_FLOAT, GL_FALSE, static_cast<GLsizei>(6 * sizeof(GLfloat)), static_cast<GLintptr>(0)},
		{color_attrib_name, 3, GL_FLOAT, GL_FALSE, static_cast<GLsizei>(6 * sizeof(GLfloat)), static_cast<GLintptr>(3 * sizeof(GLfloat))} };
	
	std::vector<shape::attrib_t> attribs_2{
		{point_attrib_name, 3, GL_FLOAT, GL_FALSE, static_cast<GLsizei>(6 * sizeof(GLfloat)), static_cast<GLintptr>(0)},
		{color_attrib_name, 3, GL_FLOAT, GL_FALSE, static_cast<GLsizei>(6 * sizeof(GLfloat)), static_cast<GLintptr>(3 * sizeof(GLfloat))} };
	
	std::vector<std::string_view> uniform_attribs_2{
		time_attrib_name };
	
	std::vector<shape> shapes;
	shapes.reserve(figures.size());
	shapes.emplace_back(std::move(figures[0]), GL_STATIC_DRAW, programs[0].get_id(), std::move(attribs_1));
	shapes.emplace_back(std::move(figures[1]), GL_STATIC_DRAW, programs[1].get_id(), std::move(attribs_2), std::move(uniform_attribs_2));

	glClearColor(background_rgba.r, background_rgba.g, background_rgba.b, background_rgba.a);

	while (!glfwWindowShouldClose(window)) {
		double time = glfwGetTime();

		glClear(GL_COLOR_BUFFER_BIT); // GL_DEPTH_BUFFER_BIT    GL_STENCIL_BUFFER_BIT
		
		shapes[0].bind_vao();
		programs[0].use_program();
		glDrawArrays(GL_TRIANGLES, 0, 3);

		
		shapes[1].bind_vao();
		programs[1].use_program();
		auto time_uniform_id = shapes[1].get_uniforms_id()[0];
		glUniform1f(time_uniform_id, time);
		glDrawArrays(GL_TRIANGLES, 0, 3);

		glfwSwapBuffers(window);
		
		poll_events(window);
	}



	for (shape& figure : shapes)
		figure.release_resources();
	for (shader_program& program : programs)
		program.release();

	glfwTerminate();

	return EXIT_SUCCESS;
}

namespace {
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