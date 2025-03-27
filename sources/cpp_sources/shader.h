#ifndef SHADER_H

#define SHADER_H

#include <string_view>
#include <string>
#include <vector>

#include "GLAD/glad.h"

class shader {
public:
	shader(std::string_view shader_file_, GLenum shader_type_);
	shader(const shader&) = delete;
	shader(shader&&) = delete;
	shader& operator=(const shader&) = delete;
	shader& operator=(shader&&) = delete;
	~shader() = default;

	void configure_shader();

	GLint shader_status(GLenum operation_param_);

	const GLuint& get_id();

private:
	void read_shader();

	std::string_view file_name;
	GLenum type;
	GLuint id;
	std::string source;
	std::string info_log;
};

class shader_program {
public:
	shader_program();
	shader_program(const shader_program&) = delete;
	shader_program(shader_program&&) = delete;
	shader_program operator=(const shader_program&) = delete;
	shader_program operator=(shader_program&&) = delete;
	~shader_program() = default;

	const GLuint& get_id();

	template<typename... GLType>
	void attach_shader(GLuint shader_id_, GLType... shaders_id_) {
		shaders_id.emplace_back(shader_id_);

		glAttachShader(id, shader_id_);

		attach_shader(shaders_id_...);
	}

	void link_program();

	GLint program_status(GLenum operation_param_);

	void delete_shaders();

	void use_program();

private:
	inline void attach_shader() {}


	GLuint id;
	std::vector<GLuint> shaders_id;
};

#endif // SHADER_H