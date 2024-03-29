#version 330 core

// Positions/Coordinates
layout (location = 0) in vec3 aPos;
// Normals (not necessarily normalized)
layout (location = 1) in vec3 aNormal;
// Colors
layout (location = 2) in vec3 aColor;
// Texture Coordinates
layout (location = 3) in vec2 aTex;


// Outputs the current position for the Fragment Shader
out vec3 crntPos;
// Outputs the normal for the Fragment Shader
out vec3 Normal;
// Outputs the color for the Fragment Shader
out vec3 color;
// Outputs the texture coordinates to the Fragment Shader
out vec2 texCoord;

out vec4 LightSpacePos;

uniform mat4 view;
uniform mat4 projection;
uniform mat4 model;
uniform mat3 normalMatrix;

// Imports the transformation matrices
uniform mat4 vmodel;
uniform mat4 translation;
uniform mat4 rotation;
uniform mat4 scale;

uniform vec4 plane;

uniform mat4 DepthMVP;

void main()
{

	// calculates current position
	crntPos = vec3(vmodel * translation * -rotation * scale * vec4(aPos, 1.0f));

	// Assigns the normal from the Vertex Data to "Normal"
	Normal = normalMatrix * aNormal;
	// Assigns the colors from the Vertex Data to "color"
	color = aColor;
	// Assigns the texture coordinates from the Vertex Data to "texCoord"
	texCoord = mat2(0.0, -1.0, 1.0, 0.0) * aTex;

	vec4 position = model * vec4(crntPos, 1.0);
	gl_ClipDistance[0] = dot(position, plane);

	LightSpacePos = DepthMVP * vec4(crntPos,1);

	gl_Position = projection * view * model * vec4(crntPos, 1.0);
	crntPos = (model * vec4(crntPos, 1.0)).xyz;
	
}