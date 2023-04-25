#pragma once

#include "../Boat/Boat.h"
#include "p6/p6.h"

class Camera {

private:
    glm::vec3 _position {};

    float x_mouse {};
    float rotationx_delta_user;

    float y_mouse {};
    float rotationy_delta_user;

    // float intro = 3.0;
    float intro = 0;

    float distance = 0.30f;
    float height = 0.12f;

public:
    Camera();

    void update(p6::Context& ctx, Boat& boat);
    glm::vec3 getPos();
};