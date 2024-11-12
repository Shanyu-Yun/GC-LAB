#version 330 core
layout (location = 0) in vec3 vPos;

uniform mat4 Model;
uniform mat4 View;
uniform mat4 Projection;
out vec3 vColors;
void main(){
    gl_Position = Projection * View * Model * vec4(vPos,1.0);
    vColors = vPos;
}