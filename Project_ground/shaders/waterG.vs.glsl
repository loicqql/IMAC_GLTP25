#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;

out vec3 vColor;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

uniform vec4 plane;

void main()
{
	vec4 position = model * vec4(aPos, 1.0);
	gl_ClipDistance[0] = dot(position, plane);

	vColor = aColor;
	gl_Position = projection * view * model * vec4(aPos, 1.0);
}