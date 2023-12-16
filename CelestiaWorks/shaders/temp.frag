#version 450

//shader input
layout (location = 0) in vec2 fragTexCoord;
layout (location = 1) in vec3 inColor;
layout (location = 2) in flat uint textureID;

layout (set = 0, binding = 0) uniform sampler2D texSamplers[2];

//output write
layout (location = 0) out vec4 outFragColor;


void main()
{
	vec4 color = texture(texSamplers[textureID],fragTexCoord);
	//outFragColor = vec4(mix(inColor,color.rgb, 0.5),color.a);
	outFragColor = color;
	
}
