#version 330 core

in vec3 vColor;

in vec2 vTexCoord;
flat in vec2 vTexId;

in vec4 LightSpacePos;

out vec4 fFragColor;

uniform sampler2D or1;
uniform sampler2D or2;
uniform sampler2D or3;
uniform sampler2D gShadowMap;

// float when_eq(int x, int y) {
//     return 1.0 - abs(sign(x - y));
// }

void main() {
    fFragColor = texture(or1, vTexCoord);
    fFragColor = mix(fFragColor, texture(or2, vTexCoord), vTexId.x);
    fFragColor = mix(fFragColor, texture(or3, vTexCoord), vTexId.y);

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

    // if(vTexId == 1) {
    //     fFragColor = texture(or1, vTexCoord);
    // }else if(vTexId == 2) {
    //     fFragColor = texture(or2, vTexCoord);
    // }else if(vTexId == 3) {
    //     fFragColor = texture(or3, vTexCoord);
    // }else if(vTexId == 4) {
    //     fFragColor = texture(or4, vTexCoord);
    // }else if(vTexId == 5) {
    //     fFragColor = texture(ar1, vTexCoord);
    // }else if(vTexId == 6) {
    //     fFragColor = texture(ar2, vTexCoord);
    // }else if(vTexId == 7) {
    //     fFragColor = texture(ar3, vTexCoord);
    // }else if(vTexId == 8) {
    //     fFragColor = texture(ar4, vTexCoord);
    // }else if(vTexId == 9) {
    //     fFragColor = texture(small, vTexCoord);
    // }else {
    //     fFragColor = vec4(vColor, 1.0);
    // }
}