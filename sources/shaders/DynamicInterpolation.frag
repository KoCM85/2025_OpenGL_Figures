#version 450 core

in vec3 frag_color;

out vec4 fragment;

uniform float time;

void main() {
	float r = abs(cos(time));
	float g = abs(sin(time));
	float b = r;

	fragment = vec4(frag_color.r + r, frag_color.g + g, frag_color.b + b, 1.0f);
}