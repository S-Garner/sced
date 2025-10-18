#version 330 core
in vec3 vColor;
out vec4 FragColor;

uniform bool useOverrideColor;
uniform vec3 uOverrideColor;

void main() {
    vec3 c = useOverrideColor ? uOverrideColor : vColor;
    FragColor = vec4(c, 1.0);
}
