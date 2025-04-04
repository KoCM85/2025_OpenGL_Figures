#ifndef SHAPE_H

#define SHAPE_H

#include <vector>
#include <tuple>
#include <string_view>
#include <typeinfo>

#include "gl_utility.h"
#include "figures_data.h"

#include "GLAD/glad.h"

class shape {
public:
	using attrib_t = std::tuple<std::string_view, GLint, GLenum, GLboolean, GLsizei, GLintptr>;

	shape(std::vector<GLfloat>&& figures_, const GLenum usage_, const GLuint& program_id_, std::vector<attrib_t>&& attribs_, std::vector<std::string_view>&& uniform_attribs_ = {});
	shape(std::vector<GLfloat>&& figures_, std::vector<GLuint>&& indices_, const GLenum usage_, const GLuint& program_id_, std::vector<attrib_t>&& attribs_, std::vector<std::string_view>&& uniform_attribs_ = {});
	shape(const shape&) = delete;
	shape(shape&&) = default;
	shape& operator=(const shape&) = delete;
	shape& operator=(shape&&) = default;
	~shape() = default;

	void bind_vao();

	const std::vector<GLint>& get_uniforms_id() const;

	size_t indices_size() const;

	void release_resources();

private:
	void configure_attribs(const GLuint& program_id_, std::vector<attrib_t>&& attribs_, std::vector<std::string_view>&& uniform_attribs_);

	std::vector<GLfloat> source;
	std::vector<GLuint> indices;
	GLuint vertex_array_id;
	GLuint vertex_buffer_id;
	GLuint element_buffer_id;
	std::vector<GLint> uniforms_id;
	std::vector<GLint> attribs_id;
};

#endif // SHAPE_H