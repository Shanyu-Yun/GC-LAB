#version 450

layout(location = 0) out vec3 fragColor;

layout(location = 0) in vec2 inPos;

layout(location = 1) in vec3 colors;

void main() {
    gl_Position = vec4(inPos, 0.0, 1.0);
    fragColor = colors;
}
