#version 450 core
in vec3 frag_color;

out vec4 fragment;

void main() {
	fragment = vec4(frag_color, 1.0f);
}