#version 330

in vec3 vColor;

out vec4 fFragColor;

void main()
{
    fFragColor = vec4(vColor, 1.);

    // float average = vColor.r + vColor.g + vColor.b/3.0;

    // fFragColor = vec4(vec3(average), 1.);
}