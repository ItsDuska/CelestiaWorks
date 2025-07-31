#version 450

// Vertex attributes
layout(location = 0) in vec2 position;
layout(location = 1) in vec2 texCoord;

// Push constant for projection matrix
layout(push_constant) uniform PushConstants {
    mat4 projection;
};

// Outputs to fragment shader
layout(location = 0) out vec2 fragTexCoord;

void main() {
    gl_Position = projection * vec4(position, 0.0, 1.0);
    fragTexCoord = texCoord;
}

