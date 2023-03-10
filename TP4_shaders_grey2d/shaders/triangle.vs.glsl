#version 330

layout(location = 3) in vec2 aVertexPosition;
layout(location = 8) in vec3 aVertexColor;

out vec3 vColor;

mat3 translate(float tx, float ty) {
    return mat3(
        vec3(1, 0, 0),
        vec3(0, 1, 0),
        vec3(tx, ty, 1)
    );
}

mat3 scale(float sx, float sy) {
    return mat3(
        vec3(sx, 0, 0),
        vec3(0, sy, 0),
        vec3(0, 0, 1)
    );
}

mat3 rotate(float a) {
    return mat3(
        vec3(cos(a), sin(a), 0),
        vec3(-sin(a), cos(a), 0),
        vec3(0, 0, 1)
    );
}

void main()
{
    vColor = aVertexColor;

    mat3 M = translate(0.5, 0.5) * rotate(3.14 / 4) * scale(0.5, 0.5);

    vec2 transformed = (M * vec3(aVertexPosition, 1.)).xy;
    gl_Position = vec4(transformed, 0., 1.);
}