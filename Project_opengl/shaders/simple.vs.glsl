#version 330 core

layout(location = 0) in vec3 in_vertex;
layout(location = 1) in vec4 in_color;
layout(location = 2) in vec2 in_texcoord;
layout(location = 3) in vec3 in_normal;

out vec4 color;
out vec3 normal;
out vec3 position;
out vec2 texcoord;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
	color = in_color;
	gl_Position = projection * view * model * vec4(in_vertex, 1);
	position = gl_Position.xyz;
	normal = normalize(mat3(projection * view * model) * in_normal);
	position = in_vertex;
	texcoord = in_texcoord;
}