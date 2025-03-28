#include <filesystem>
#include <fstream>

#include "shader.h"

#include "gl_utility.h"

// shader
shader::shader(std::string_view shader_file_, GLenum shader_type_) : file_name(shader_file_), type(shader_type_) {
	read_shader();

	id = glCreateShader(type);
}

void shader::configure_shader() {
	const GLchar* const shader_source_p = source.c_str();

	glShaderSource(id, 1, &shader_source_p, nullptr);
	glCompileShader(id);
}

GLint shader::shader_status(GLenum operation_param_) {
	GLint shader_status;

	glGetShaderiv(id, operation_param_, &shader_status);

	if (!shader_status) {
		const GLsizei buf_size = 512;
		GLchar info[buf_size];

		glGetShaderInfoLog(id, buf_size, nullptr, info);

		logger(info);
	}

	return shader_status;
}

const GLuint& shader::get_id() {
	return id;
}

void shader::read_shader() {
	std::filesystem::path shader_full_path(std::filesystem::current_path());
	shader_full_path += std::filesystem::path::preferred_separator;
	shader_full_path += "shaders";
	shader_full_path += std::filesystem::path::preferred_separator;
	shader_full_path += file_name;

	std::ifstream stream(shader_full_path, std::ios::binary | std::ios::in);
	if (stream.is_open()) {
		std::stringstream shader;

		shader << stream.rdbuf();

		source = std::move(shader.str());
		stream.close();
	}
	else {
		std::string_view log_info("File can't be opened");

		logger(log_info, shader_full_path.generic_string());
	}
}
// shader

// ----------------------------------------------------------------------------------------------------------------------------------------------------

// shader_program
shader_program::shader_program() {
	id = glCreateProgram();
	shaders_id.reserve(10);
}

const GLuint& shader_program::get_id() {
	return id;
}

void shader_program::link_program() {
	glLinkProgram(id);
}

GLint shader_program::program_status(GLenum operation_param_) {
	GLint program_status;

	glGetProgramiv(id, operation_param_, &program_status);

	if (!program_status) {
		const GLsizei buf_size = 512;
		GLchar info[buf_size];

		glGetProgramInfoLog(id, buf_size, nullptr, info);

		logger(info);
	}

	return program_status;
}

void shader_program::delete_shaders() {
	for (GLuint& shader_id : shaders_id)
		glDeleteShader(shader_id);
}

void shader_program::use_program() {
	glUseProgram(id);
}

void shader_program::release() {
	glDeleteProgram(id);
}

// shader_program