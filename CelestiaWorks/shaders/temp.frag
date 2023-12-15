#version 450

//shader input
layout (location = 0) in vec2 fragTexCoord;
layout (location = 1) in vec3 inColor;

layout (binding = 0) uniform sampler2D texSampler;

//output write
layout (location = 0) out vec4 outFragColor;


void main()
{
	outFragColor = texture(texSampler,fragTexCoord);
	//outFragColor = vec4(inColor,1.0f);
	
}
