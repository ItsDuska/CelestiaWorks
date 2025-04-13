#version 450

layout (location = 0) in vec2 fragTexCoord;
layout (location = 1) in vec3 inColor;

layout (set = 0, binding = 0) uniform sampler2D texSampler;

layout (location = 0) out vec4 outFragColor;

void main()
{
	vec4 color = texture(texSampler, fragTexCoord);
	outFragColor = color;
	vec4 tempColor = vec4(mix(color.xyz, inColor, 1), color.w);
}