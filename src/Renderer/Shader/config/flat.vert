#version 330 core
layout(location=0) in vec2 aPos;
layout(location=1) in vec3 aColor;

uniform mat4 modelMatrix;
uniform mat4 viewProjMatrix;

out vec3 vColor;

void main() {
    gl_Position = viewProjMatrix * modelMatrix * vec4(aPos, 0.0, 1.0);
    vColor = aColor;
}