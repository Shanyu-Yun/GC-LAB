#version 330 core

uniform vec3 Colors;
layout(location = 0) out vec4 FragColor;
in vec3 vColors;

void main(){
    vec3 color;
     float t = clamp((vColors.y + 2.0) / 4.0, 0.0, 1.0);
     t = 1.0 - t;
    if (t< 0.33) {
        float localT = t / 0.33;
        color = mix(vec3(0.8, 0.0, 0.0), vec3(0.8, 0.4, 0.0), localT);
    } else if (t < 0.66) {
        float localT = (t - 0.33) / 0.33;
        color = mix(vec3(0.8, 0.4, 0.0), vec3(0.0, 0.6, 0.6), localT);
    } else {
        float localT = (t - 0.66) / 0.34;
        color = mix(vec3(0.0, 0.6, 0.6), vec3(0.0, 0.0, 0.4), localT);
    }
    FragColor = vec4(color,1.0);
}