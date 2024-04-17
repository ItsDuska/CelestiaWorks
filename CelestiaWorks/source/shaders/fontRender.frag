#version 450
layout (location = 0) in vec2 inTextCoords;
layout (location = 1) in vec4 inColor;

layout (location = 0) out vec4 outFragColor;

layout (set = 0, binding = 0) uniform sampler2D text;

void main()
{
	vec4 sampled = vec4(1.0, 1.0, 1.0, texture(text, inTextCoords).r);
	outFragColor = inColor * sampled;
}