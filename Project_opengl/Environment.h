#pragma once

#include "./utils.h"
#include "glm/fwd.hpp"

inline glm::mat4 getModelCasleCentre() {

    glm::mat4 model = glm::mat4(1.0);
    model = glm::translate(model, glm::vec3(-0.5, -0.01, 0.85));
    model = glm::rotate(model, p6::PI, glm::vec3(1.0f, 0.0f, 0.0f));
    model = glm::rotate(model, -p6::PI / 2.f, glm::vec3(0.0f, 1.0f, 0.0f));
    model = glm::scale(model, glm::vec3(0.012));

    return model;
}