#ifndef SHADER_H

#define SHADER_H

#include <string_view>
#include <string>
#include <utility>

#include "GLAD/glad.h"

class shader {
public:
	shader(std::string_view shader_file_, GLenum shader_type_);

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

#endif // SHADER_H