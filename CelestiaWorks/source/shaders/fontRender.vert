#version 450
layout (location = 0) in vec3 inPosition;
layout (location = 1) in vec4 inColor;
layout (location = 2) in vec2 inTexCoord;

layout (location = 0) out vec2 outTexCoord;
layout (location = 1) out vec4 outColor;

layout (push_constant) uniform pushValues {
	mat4 projection;
} pushConstants;

void main()
{
	gl_Position = pushConstants.projection * vec4(inPosition, 1.0);
	
	outTexCoord = inTexCoord;
	outColor = inColor;
}