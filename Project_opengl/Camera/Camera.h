#pragma once

#include "../Boat/Boat.h"
#include "glm/fwd.hpp"
#include "p6/p6.h"

class Camera {

private:
    glm::vec3 _position {};
    glm::vec3 _targetPosition {};

    float x_mouse {};
    float rotationx_delta_user;

    float y_mouse {};
    float rotationy_delta_user;

    float distance = 0.30f;
    float defaultHeight = 0.12f;

public:
    Camera();

    void update(p6::Context& ctx, glm::vec3 targetPosition, glm::vec3 targetRotation);
    glm::vec3 getPos();
    glm::vec3 getTargetPos();
};