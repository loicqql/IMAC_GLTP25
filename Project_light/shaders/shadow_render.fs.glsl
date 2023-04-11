#version 330 core

// Ouput data
out vec3 fFragColor;

uniform sampler2D gShadowMap;
// uniform sampler2DShadow  gShadowMap;

in vec4 LightSpacePos;
in vec3 vColor;

void main(){

    vec3 ProjCoords = LightSpacePos.xyz / LightSpacePos.w;
    vec2 UVCoords;
    UVCoords.x = 0.5 * ProjCoords.x + 0.5;
    UVCoords.y = 0.5 * ProjCoords.y + 0.5;
    float z = 0.5 * ProjCoords.z + 0.5;
    float Depth = texture(gShadowMap, UVCoords).x;
    
    float bias = 0.005;

    if(Depth + bias < z) {
        fFragColor = vColor - vec3(1) * 0.1;
    }else {
        fFragColor = vColor;
    }

    
}