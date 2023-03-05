#pragma once
#include "glm/geometric.hpp"
#include <glm/glm.hpp>

void vec2_limit(glm::vec2 &vec, glm::vec2 limit) {
    vec.x = vec.x > limit.x ? limit.x : vec.x;
    vec.y = vec.y > limit.y ? limit.y : vec.y;
    vec.x = vec.x < limit.x * -1.0 ? limit.x * -1.0 : vec.x;
    vec.y = vec.y < limit.y * -1.0 ? limit.x * -1.0 : vec.y;
}

float vec2_get_direction(glm::vec2 vec) {
    return std::atan(vec.y /vec.x);
}