#version 450

layout (location = 0) in vec2 position;
layout (location = 1) in vec2 texCoord;
layout (location = 2) in vec3 color;
layout (location = 3) in uint texIndex;

layout (location = 0) out vec2 fragTexCoord;
layout (location = 1) out vec3 outColor;


layout(std140, set = 0, binding = 1) readonly buffer ObjectBuffer {
	vec2 transforms[];
} transformBuffer;

layout (push_constant) uniform pushValues {
	mat4 projection;
} pushConstants;

void main()
{

    vec2 translation = transformBuffer.transforms[texIndex];

    // Lisää sijainnin muutos vertexin sijaintiin
    vec2 newPosition = position + translation;

    gl_Position = pushConstants.projection * vec4(newPosition.xy, 0.0, 1.0);

    fragTexCoord = texCoord;
	outColor = color;
}
