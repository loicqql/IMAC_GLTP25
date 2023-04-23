#version 330 core

in vec3 vColor;

in vec2 vTexCoord;
flat in vec2 vTexId;

in vec4 LightSpacePos;

out vec4 fFragColor;

uniform sampler2D ar1;
uniform sampler2D ar2;
uniform sampler2D ar3;
uniform sampler2D gShadowMap;

void main() {
    fFragColor = texture(ar1, vTexCoord);
    fFragColor = mix(fFragColor, texture(ar2, vTexCoord), vTexId.x);
    fFragColor = mix(fFragColor, texture(ar3, vTexCoord), vTexId.y);

    vec3 ProjCoords = LightSpacePos.xyz / LightSpacePos.w;
    vec2 UVCoords;
    UVCoords.x = 0.5 * ProjCoords.x + 0.5;
    UVCoords.y = 0.5 * ProjCoords.y + 0.5;
    float z = 0.5 * ProjCoords.z + 0.5;
    float Depth = texture(gShadowMap, UVCoords).x;
    
    float bias = 0.005;

    if(Depth + bias < z) {
        fFragColor = fFragColor - vec4(1) * 0.2;
    }
}