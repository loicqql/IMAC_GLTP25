#pragma once
#include "glm/geometric.hpp"
#include <glm/glm.hpp>

void vec3_limit(glm::vec3 &vec, glm::vec3 limit) {
    vec.x = vec.x > limit.x ? limit.x : vec.x;
    vec.y = vec.y > limit.y ? limit.y : vec.y;
    vec.z = vec.z > limit.z ? limit.z : vec.z;

    vec.x = vec.x < limit.x * -1.0 ? limit.x * -1.0 : vec.x;
    vec.y = vec.y < limit.y * -1.0 ? limit.x * -1.0 : vec.y;
    vec.z = vec.z < limit.z * -1.0 ? limit.z * -1.0 : vec.z;
}

float vec2_get_direction(glm::vec2 vec) {
    return std::atan(vec.y / vec.x);
}