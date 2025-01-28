#version 450

layout (location = 0) in vec2 fragTexCoord;
layout (location = 1) in vec3 inColor;

layout (set = 0, binding = 0) uniform sampler2D texSampler;

layout (location = 0) out vec4 outFragColor;


void main()
{
	float glyphAlpha = texture(texSampler, fragTexCoord).r;

    if (glyphAlpha == 0.0)
    {
        discard; // Älä piirrä tätä fragmenttia
    }

    outFragColor = vec4(inColor, glyphAlpha);

}
