#version 450

layout(location = 0) in vec3 inPosition;  // 顶点位置
layout(location = 1) in vec3 fgColor;    
layout(location = 2) in vec3 inNormal;  // 顶点法线

layout(set = 0, binding = 0) uniform UBO {
    mat4 model;
    mat4 view;
    mat4 proj;
} ubo;

layout(location = 0) out vec3 fragPos;   // 世界坐标中的顶点位置
layout(location = 1) out vec3 fragNormal; // 世界坐标中的法线方向
layout(location = 2) out vec3 fragColor;

void main() {
    // 将顶点坐标从本地坐标转换到世界坐标
    vec4 worldPosition = ubo.model * vec4(inPosition, 1.0);
    fragPos = worldPosition.xyz;

    // 将法线向量转换到世界坐标系
    fragNormal = mat3(transpose(inverse(ubo.model))) * inNormal;

    // 应用 view 和 projection 矩阵，转换到裁剪空间
    gl_Position = ubo.proj * ubo.view * worldPosition;

    fragColor = fgColor;
}
