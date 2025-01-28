#version 450

layout (location = 0) in vec2 fragTexCoord;
layout (location = 1) in vec3 inColor;

layout (set = 0, binding = 0) uniform sampler2D texSampler;

layout (location = 0) out vec4 outFragColor;


void main()
{
	//vec4 sampled = vec4(vec3(texture(texSampler, fragTexCoord).r), 1.0); // old

	//outFragColor = vec4(inColor, 1.0) * sampled; // old

	float glyphAlpha = texture(texSampler, fragTexCoord).r;

    // Tee musta tausta läpinäkyvä
    if (glyphAlpha == 0.0)
    {
        discard; // Älä piirrä tätä fragmenttia
    }

    // Lopputulos glyfin värillä ja alfalla
    outFragColor = vec4(inColor, glyphAlpha);

}
