#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;
layout (location = 2) in vec2 aTexCoord;
layout (location = 3) in int aTexId;

out vec3 vColor;

out vec2 vTexCoord;
flat out int vTexId;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
	vColor = aColor;
	vTexCoord = aTexCoord;
	vTexId = aTexId;
	gl_Position = projection * view * model * vec4(aPos, 1.0);
}