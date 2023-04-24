#pragma once
#include "glm/geometric.hpp"
#include "p6/p6.h"
#include <glm/glm.hpp>

inline void vec3_limit(glm::vec3& vec, glm::vec3 limit) {
    vec.x = vec.x > limit.x ? limit.x : vec.x;
    vec.y = vec.y > limit.y ? limit.y : vec.y;
    vec.z = vec.z > limit.z ? limit.z : vec.z;

    vec.x = vec.x < limit.x * -1.0 ? limit.x * -1.0 : vec.x;
    vec.y = vec.y < limit.y * -1.0 ? limit.x * -1.0 : vec.y;
    vec.z = vec.z < limit.z * -1.0 ? limit.z * -1.0 : vec.z;
}

inline float float_limit(float f, float limit, bool floor) {
    if (floor) {
        f = f > limit ? f : limit;
    } else {
        f = f < limit ? f : limit;
    }

    return f;
}

inline float vec2_get_direction(glm::vec2 vec) {
    return std::atan(vec.y / vec.x);
}

inline float float_map(float x, float x_min, float x_max) {
    return (x_max - x_min) * x + x_min;
}

inline glm::vec3 color_map(float x, float x_min, float x_max, glm::vec3 color_min, glm::vec3 color_max) {
    float _x = float_map(x, x_min, x_max);

    return glm::vec3(float_map(_x, color_max.x, color_min.x), float_map(_x, color_max.y, color_min.y), float_map(_x, color_max.z, color_min.z));
}

void loadTexture(GLuint& textureId, auto filename) {
    const img::Image image = p6::load_image_buffer(filename);

    glGenTextures(1, &textureId);
    glBindTexture(GL_TEXTURE_2D, textureId);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image.width(), image.height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, image.data());

    glBindTexture(GL_TEXTURE_2D, 0);
}