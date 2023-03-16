#pragma once

#include "glm/fwd.hpp"
#include "glm/trigonometric.hpp"
#include "utils.h"
#include <glm/glm.hpp>
#include <iostream>
#include "p6/p6.h"

#include <math.h>

class Camera {
    
    private : 
        glm::vec3 _position{};
        glm::vec3 _rotation{};

    public : 

        void init() {
            this->_position = glm::vec3(0);
            this->_rotation = glm::vec3(0);
        }

        void update(glm::vec3 position, glm::vec3 rotation) {

            // glm::vec2 mouse = ctx.mouse();


            this->_position = {glm::cos(rotation.y) * 0.08 + position.x, -0.9f, glm::sin(rotation.y) * 0.08 + position.z};
            this->_rotation = {3.14 / 4, rotation.y, 0.0};


        }

        glm::vec3 getPos() {
            return this->_position;
        }

        glm::vec3 getRot() {
            return this->_rotation;
        }
};