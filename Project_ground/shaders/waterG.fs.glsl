#version 330 core

in vec3 vColor;

// layout(location = 0) out vec4 fFragColor;

out vec4 fFragColor;

void main()
{
    // fFragColor = vec3(1.0, 0.0, 1.0);
	fFragColor = vec4(vColor, 1.);
}