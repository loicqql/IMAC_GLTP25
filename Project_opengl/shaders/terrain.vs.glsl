#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;
layout (location = 2) in vec2 aTexCoord;
layout (location = 3) in vec2 aTexId;
layout (location = 4) in vec3 aNormal;

out vec3 vColor;
out vec4 LightSpacePos;

out vec2 vTexCoord;
flat out vec2 vTexId;

out vec3 Normal;

out vec3 crntPos;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform mat3 normalMatrix;

uniform vec4 plane;

uniform mat4 DepthMVP;

void main()
{

	Normal = normalMatrix * aNormal;

	vec4 position = model * vec4(aPos, 1.0);
	gl_ClipDistance[0] = dot(position, plane);

	vColor = aColor;
	vTexCoord = aTexCoord;
	vTexId = aTexId;
	gl_Position = projection * view * model * vec4(aPos, 1.0);
	LightSpacePos = DepthMVP * vec4(aPos,1);
	crntPos = gl_Position.xyz;
}