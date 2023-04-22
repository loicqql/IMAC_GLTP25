#version 330 core

in vec3 vColor;

in vec2 vTexCoord;
flat in vec2 vTexId;

out vec4 fFragColor;

uniform sampler2D small;


void main() {
    fFragColor = texture(small, vTexCoord);
    // fFragColor = vec4(vColor, 1.0);
}