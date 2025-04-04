#version 450 core

layout(location = 0) in vec3 point;

out float time_to_frag;

uniform float time;

void main() {
	float x = sin(time);
	float y = sin(time);

	gl_Position = vec4(point.x + x, point.y + y, point.z, 1.0f);
	time_to_frag = time;
}