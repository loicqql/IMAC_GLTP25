#version 330 core

in vec3 vColor;

in vec2 vTexCoord;
flat in vec2 vTexId;

out vec4 fFragColor;

uniform sampler2D ar1;
uniform sampler2D ar2;
uniform sampler2D ar3;

void main() {
    fFragColor = texture(ar1, vTexCoord);
    fFragColor = mix(fFragColor, texture(ar2, vTexCoord), vTexId.x);
    fFragColor = mix(fFragColor, texture(ar3, vTexCoord), vTexId.y);
}