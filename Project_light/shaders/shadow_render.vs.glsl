#version 330 core

// Input vertex data, different for all executions of this shader.
layout(location = 0) in vec3 vertexPosition_modelspace;
layout (location = 1) in vec3 aColor;

// Values that stay constant for the whole mesh.
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

uniform mat4 DepthMVP;

out vec4 LightSpacePos;
out vec3 vColor;

void main(){
    gl_Position = projection * view * model * vec4(vertexPosition_modelspace,1);
    LightSpacePos = DepthMVP * vec4(vertexPosition_modelspace,1);
    vColor = aColor;
}