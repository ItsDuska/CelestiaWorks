#version 450

layout (location = 0) in vec2 position;
layout (location = 1) in vec2 texCoord;
layout (location = 2) in vec3 color;

layout (location = 0) out vec3 outColor;

layout (push_constant) uniform constants {
	mat4 transform;
	mat4 projection;
} pushConstants;

void main()
{
	gl_Position = pushConstants.projection * pushConstants.transform * vec4(position.xy, 0.0, 1.0);
	outColor = color;
}
