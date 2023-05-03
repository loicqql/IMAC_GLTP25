#include "Camera.h"
#include "glm/fwd.hpp"
#include "glm/trigonometric.hpp"
#include <cmath>
#include <glm/glm.hpp>

Camera::Camera() {
    this->_position = glm::vec3(0);
    rotationx_delta_user = 0.f;
    rotationy_delta_user = 0.f;
}

void Camera::update(p6::Context& ctx, glm::vec3 targetPosition, glm::vec3 targetRotation) {

    float force_reset_rotationx_delta_user = 0.005f;

    _targetPosition = targetPosition;
    glm::vec3 position = targetPosition;
    glm::vec3 rotation = targetRotation;

    glm::vec2 mouse = ctx.mouse();

    if (ctx.key_is_pressed(GLFW_KEY_G)) {
        rotationx_delta_user = 0;
        rotationy_delta_user = 0;
    }

    if (ctx.mouse_button_is_pressed(p6::Button::Left)) {
        rotationx_delta_user -= x_mouse - mouse.x;
        rotationy_delta_user += (y_mouse - mouse.y) * 0.5f;
    }

    /*

    if (!ctx.mouse_button_is_pressed(p6::Button::Left) && (ctx.key_is_pressed(GLFW_KEY_S) || ctx.key_is_pressed(GLFW_KEY_L))) {
        if (rotationx_delta_user != 0.f) {
            if (std::fabs(rotationx_delta_user) < force_reset_rotationx_delta_user) { // near 0
                rotationx_delta_user = 0.f;
            } else {
                if (rotationx_delta_user > 0) {
                    rotationx_delta_user -= force_reset_rotationx_delta_user;
                } else {
                    rotationx_delta_user += force_reset_rotationx_delta_user;
                }
            }
        }
    }

    */

    x_mouse = mouse.x;
    y_mouse = mouse.y;

    float x = glm::cos(rotation.y + rotationx_delta_user) * distance + position.x;
    float z = glm::sin(rotation.y + rotationx_delta_user) * distance + position.z;
    float y = _targetPosition.y + defaultHeight + rotationy_delta_user;

    _position = { x, y, z };

    const float OCEAN_HEIGHT = 0.0f; // to do

    if (_position.y < OCEAN_HEIGHT + 0.01f) {
        _position.y = OCEAN_HEIGHT + 0.01f;
    }
}

glm::vec3 Camera::getPos() {
    return this->_position;
}

glm::vec3 Camera::getTargetPos() {
    return _targetPosition;
}