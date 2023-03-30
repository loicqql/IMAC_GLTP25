#pragma once
#include "glm/geometric.hpp"
#include <glm/glm.hpp>

inline void vec3_limit(glm::vec3 &vec, glm::vec3 limit) {
    vec.x = vec.x > limit.x ? limit.x : vec.x;
    vec.y = vec.y > limit.y ? limit.y : vec.y;
    vec.z = vec.z > limit.z ? limit.z : vec.z;

    vec.x = vec.x < limit.x * -1.0 ? limit.x * -1.0 : vec.x;
    vec.y = vec.y < limit.y * -1.0 ? limit.x * -1.0 : vec.y;
    vec.z = vec.z < limit.z * -1.0 ? limit.z * -1.0 : vec.z;
}

inline float float_limit(float f, float limit, bool floor) {
    if(floor) {
        f = f > limit ? f : limit;
    }else {
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