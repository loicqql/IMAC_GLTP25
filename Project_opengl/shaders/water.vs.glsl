#version 330 core

layout (location = 0) in vec3 aPos;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

uniform vec3 lightPosition;

out vec4 clipSpace;
out vec2 textureCoords;
out vec3 crntPos;

const float tiling = 4;

void main()
{
	vec4 worldPosition = model * vec4(aPos, 1.0);
	clipSpace = projection * view * worldPosition;
	gl_Position = clipSpace;
	textureCoords = vec2(aPos.x, aPos.y) * tiling;
	crntPos = worldPosition.xyz;
}