#version 450

// 顶点输入：顶点位置
layout(location = 0) in vec2 inPosition;

// 顶点输入：实例颜色
layout(location = 1) in vec3 inColor;

// 顶点输入：实例位置
layout(location = 2) in vec3 inInstancePosition;

//顶点输入：实例颜色
layout(location = 3) in vec3 instanceColor;

// 输出到片段着色器的颜色
layout(location = 0) out vec3 fragColor;

void main() {
    // 计算顶点的最终位置，加入实例位置
    gl_Position = vec4(inPosition + inInstancePosition.xy, 0.0, 1.0);
    
    // 使用实例颜色
    fragColor = inColor + instanceColor;
}