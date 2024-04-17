#version 450

layout (location = 0) in vec2 fragTexCoord;
layout (location = 1) in vec3 inColor;

layout (set = 0, binding = 0) uniform sampler2D texSampler;

layout (location = 0) out vec4 outFragColor;


void main()
{
	//vec4 sampled = vec4(1.0,1.0,1.0, texture(texSampler,fragTexCoord).r);
	//float r = texture(texSampler,fragTexCoord).r;
	//outFragColor = vec4(inColor,1.0)*sampled;
	//outFragColor = vec4(r,r,r,1.0);

	vec4 sampled = vec4(vec3(texture(texSampler,fragTexCoord).r),1.0);

	outFragColor = vec4(inColor,1.0)*sampled;
}

// tää toimii
//float r = texture(texSampler,fragTexCoord).r;
//outFragColor = vec4(r,r,r,1.0);