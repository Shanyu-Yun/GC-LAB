#version 450

layout(location = 0) in vec3 fragPos;    // 顶点的世界坐标
layout(location = 1) in vec3 fragNormal; // 顶点的世界法线
layout(location = 2) in vec3 fragColor;

layout(set = 0, binding = 1) uniform Light {
    vec3 ambientColor;  // 环境光颜色
} light;

layout(location = 0) out vec4 outColor;

void main() {
    // 环境光的计算：ambient = ambientColor * objectColor
    vec3 objectColor = fragColor;  // 物体的基本颜色
    vec3 ambient = light.ambientColor * objectColor;

    // 最终颜色输出
    outColor = vec4(ambient, 1.0);
}