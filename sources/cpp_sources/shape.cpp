#include <limits>

#include "shape.h"

shape::shape(std::vector<GLfloat>&& figures_, const GLenum usage_, const GLuint& program_id_, std::vector<attrib_t>&& attribs_, std::vector<std::string_view>&& uniform_attribs_) : source(figures_) {
	const GLsizeiptr source_size = source.size() * sizeof(decltype(source)::value_type);
	element_buffer_id = std::numeric_limits<GLuint>::max();

	glGenVertexArrays(1, &vertex_array_id);
	glGenBuffers(1, &vertex_buffer_id);

	bind_vao();
	glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer_id);

	glBufferData(GL_ARRAY_BUFFER, source_size, source.data(), usage_);

	configure_attribs(program_id_, std::move(attribs_), std::move(uniform_attribs_));
}

shape::shape(std::vector<GLfloat>&& figures_, std::vector<GLuint>&& indices_, const GLenum usage_, const GLuint& program_id_, std::vector<attrib_t>&& attribs_, std::vector<std::string_view>&& uniform_attribs_) : source(figures_), indices(indices_) {
	const GLsizeiptr source_size = source.size() * sizeof(decltype(source)::value_type);
	const GLsizeiptr indices_size = indices.size() * sizeof(decltype(indices)::value_type);

	glGenVertexArrays(1, &vertex_array_id);
	glGenBuffers(1, &vertex_buffer_id);
	glGenBuffers(1, &element_buffer_id);

	bind_vao();
	glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer_id);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, element_buffer_id);

	glBufferData(GL_ARRAY_BUFFER, source_size, source.data(), usage_);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices_size, indices.data(), usage_);

	configure_attribs(program_id_, std::move(attribs_), std::move(uniform_attribs_));
}

void shape::bind_vao() {
	glBindVertexArray(vertex_array_id);
}

const std::vector<GLint>& shape::get_uniforms_id() const {
	return uniforms_id;
}

size_t shape::indices_size() const {
	return indices.size();
}

void shape::release_resources() {
	for (const GLuint& attrib_id : attribs_id)
		glDisableVertexAttribArray(attrib_id);

	glDeleteVertexArrays(1, &vertex_array_id);
	glDeleteBuffers(1, &vertex_buffer_id);
	if (!indices.empty())
		glDeleteBuffers(1, &element_buffer_id);
	source.clear();
	indices.clear();
}

void shape::configure_attribs(const GLuint& program_id_, std::vector<attrib_t>&& attribs_, std::vector<std::string_view>&& uniform_attribs_) {
	uniforms_id.reserve(uniform_attribs_.size());
	attribs_id.reserve(attribs_.size());


	for (const std::string_view& uniform_attrib_name : uniform_attribs_) {
		GLint uniform_id = glGetUniformLocation(program_id_, uniform_attrib_name.data());

		uniforms_id.push_back(uniform_id);
	}

	for (const attrib_t& attrib : attribs_) {
		const void* offset = reinterpret_cast<const void*>(std::get<5>(attrib));
		GLuint attrib_id = glGetAttribLocation(program_id_, std::get<0>(attrib).data());

		glEnableVertexAttribArray(attrib_id);
		glVertexAttribPointer(attrib_id, std::get<1>(attrib), std::get<2>(attrib), std::get<3>(attrib), std::get<4>(attrib), offset);

		attribs_id.push_back(attrib_id);
	}
}