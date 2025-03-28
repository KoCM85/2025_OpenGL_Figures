#version 450 core
layout(location = 0) in vec3 point;
layout(location = 1) in vec3 color;

out vec3 frag_color;

void main() {
	gl_Position = vec4(point, 1.0f);

	frag_color = color;
}