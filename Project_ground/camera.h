#pragma once

#include "glm/fwd.hpp"
#include "glm/trigonometric.hpp"
#include <glm/glm.hpp>

#include "p6/p6.h"

#include <math.h>

#include "utils.h"

class Camera {
    
    private : 
        glm::vec3 _position{};

    public : 

        float distance = 0.30f;
        float height = 0.12f;

        void init() {
            this->_position = glm::vec3(0);
        }

        void update(glm::vec3 position, glm::vec3 rotation) {

            // glm::vec2 mouse = ctx.mouse();


            this->_position = {glm::cos(rotation.y) * distance + position.x, height, glm::sin(rotation.y) * distance + position.z};


        }

        glm::vec3 getPos() {
            return this->_position;
        }
};