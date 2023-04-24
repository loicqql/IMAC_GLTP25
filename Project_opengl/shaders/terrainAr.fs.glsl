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

    float bias = 0.0003;
    float visibility = 0.0;
    vec2 poissonDisk[4] = vec2[](
        vec2( -0.09420163, -0.03990622 ),
        vec2( 0.094558609, -0.07689073 ),
        vec2( -0.009418411, -0.0929389 ),
        vec2( 0.03449594, 0.02938777 )
    );

    for (int i=0;i<2;i++){
        if (texture(gShadowMap, UVCoords + poissonDisk[i]/700.0 ).x  + bias < z) {
            visibility += 0.05;
        }
    }
    fFragColor = fFragColor - vec4(1) * visibility;
}