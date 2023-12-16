#version 450

layout (location = 0) in vec2 position;
layout (location = 1) in vec2 texCoord;
layout (location = 2) in vec3 color;
layout (location = 3) in uint texIndex;

layout (location = 0) out vec2 fragTexCoord;
layout (location = 1) out vec3 outColor;
layout (location = 2) out flat uint textureID;

layout (set = 0, binding = 1) uniform constants
{
	mat4 transform;
	mat4 projection;
} matrix;

layout (push_constant) uniform textureIDPush {
	uint texID;
} pushConstants;

void main()
{
	gl_Position = matrix.projection * matrix.transform * vec4(position.xy, 0.0, 1.0);
	fragTexCoord = texCoord;
	outColor = color;
	textureID = texIndex;
}
