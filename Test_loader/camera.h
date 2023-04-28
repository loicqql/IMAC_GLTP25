#pragma once

#include "glm/fwd.hpp"
#include "glm/trigonometric.hpp"
#include <cmath>
#include <glm/glm.hpp>

#include "p6/p6.h"

#include <cmath>

class Camera {

private:
    glm::vec3 _position {};

    float x_mouse;
    float rotationx_delta_user;

    float y_mouse;
    float rotationy_delta_user;

    float distance = 0.30f;
    float height = 0.12f;

public:
    void init() {
        this->_position = glm::vec3(0);
        rotationx_delta_user = 0.f;
        rotationy_delta_user = 0.f;
    }

    void update(p6::Context& ctx) {

        float force_reset_rotationx_delta_user = 0.005f;
        glm::vec3 position = glm::vec3(5.0, 5.0, 0.0);
        glm::vec3 rotation { 0 };

        glm::vec2 mouse = ctx.mouse();

        if (ctx.key_is_pressed(GLFW_KEY_G)) {
            rotationx_delta_user = 0;
            rotationy_delta_user = 0;
        }

        if (ctx.mouse_button_is_pressed(p6::Button::Left)) {
            rotationx_delta_user -= x_mouse - mouse.x;
            rotationy_delta_user += (y_mouse - mouse.y) * 0.5f;
        }

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

        x_mouse = mouse.x;
        y_mouse = mouse.y;

        // if((height + rotationy_delta_user) < position.y) {
        //     rotationy_delta_user =  position.y - height;
        // }

        this->_position = { glm::cos(rotation.y + rotationx_delta_user) * distance + position.x, height + rotationy_delta_user, glm::sin(rotation.y + rotationx_delta_user) * distance + position.z };
    }

    glm::vec3 getPos() {
        return this->_position;
    }
};