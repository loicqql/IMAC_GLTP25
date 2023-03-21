#version 330 core
in vec3 normal;
in vec3 position;
in vec2 texcoord;

uniform sampler2D tex;
uniform vec3 sun_position;
uniform vec3 sun_color;

out vec4 color;
void main() {
	float lum = max(dot(normal, normalize(vec3(3.0, 10.0, -5.0))), 0.0);
	color = texture(tex, texcoord) * vec4((0.3 + 0.7 * lum) * vec3(1.0), 1.0);
}