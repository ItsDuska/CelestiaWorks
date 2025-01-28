#version 450
#extension GL_EXT_nonuniform_qualifier : enable

layout (location = 0) in vec2 fragTexCoord;
layout (location = 1) in vec3 inColor;
layout (location = 2) in flat uint textureID;

layout (set = 0, binding = 0) uniform sampler2D texSamplers[];

layout (location = 0) out vec4 outFragColor;


void main()
{
	vec4 color = texture(texSamplers[nonuniformEXT(textureID)],fragTexCoord);
	outFragColor = color;
	
}
