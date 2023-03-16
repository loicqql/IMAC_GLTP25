#version 330 core

in vec3 vColor;

out vec4 fFragColor;

void main()
{
    fFragColor = vec4(vColor, 1.);
	// fFragColor = vec4(1.);
}