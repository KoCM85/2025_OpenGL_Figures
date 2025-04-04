#version 450 core

out vec4 fragment;

uniform float time;

void main() {
	float r = abs(cos(time));
	float g = abs(sin(time));
	float b = r;

	fragment = vec4(r, g, b, 1.0f);
}