#version 450 core

in float time_to_frag;

out vec4 fragment;

void main() {
	float r = abs(sin(time_to_frag));
	float g = abs(cos(time_to_frag));
	float b = r;

	fragment = vec4(r, g, b, 1.0f);
}