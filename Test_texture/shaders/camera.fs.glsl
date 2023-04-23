#version 330 core

in vec3 vColor;

in vec2 vTexCoord;
flat in int vTexId;

out vec4 fFragColor;

uniform sampler2D or1;
uniform sampler2D or2;
uniform sampler2D or3;
uniform sampler2D or4;

void main()
{
    // fFragColor = vec4(vColor, 1.);
    if(vTexId == 1) {
        fFragColor = texture(or1, vTexCoord);
    }else if(vTexId == 2) {
        fFragColor = texture(or2, vTexCoord);
    }else if(vTexId == 3) {
        fFragColor = texture(or3, vTexCoord);
    }else if(vTexId == 4) {
        fFragColor = texture(or4, vTexCoord);
    }else {
        fFragColor = vec4(vColor, 1.0);
    }
}