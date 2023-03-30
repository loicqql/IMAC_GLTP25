#pragma once

#include "glm/ext/matrix_transform.hpp"
#include "glm/ext/quaternion_transform.hpp"
#include "glm/fwd.hpp"

#include "./loaders/gltf/loaderGLTF.h"

#include "glm/gtc/type_ptr.hpp"
#include "p6/p6.h"

class Boat {
    private : 
        glm::vec3 _position{};
        glm::vec3 _rotation{};

        loaderGLTF main;
        loaderGLTF rouge;
        loaderGLTF bleu;

        float width = 0.02;
        float speed = 0.0;
        float rotBleu = 0.0;
        float rotRouge = 0.0;
    
    public: 

        void init() {

            _position = {0.5, 0, 0.1};
            _rotation = {0, 0, 0};

            main.init("./assets/models/boat/boat.gltf");
            rouge.init("./assets/models/boat/rouge.gltf");
            bleu.init("./assets/models/boat/bleu.gltf");

        }

        void update(p6::Context &ctx) {

            float forceRot = 0.01;
            float limit = 0.0017; 

            float coeffRot = 0.01;

            if(ctx.key_is_pressed(GLFW_KEY_S) && ctx.key_is_pressed(GLFW_KEY_L)) {
                speed += 0.00005;
                rotRouge += coeffRot;
                rotBleu += coeffRot;
            }else if(ctx.key_is_pressed(GLFW_KEY_S)) {
                _rotation.y += forceRot;
                rotRouge += coeffRot;
                speed -= 0.000001;
            }else if(ctx.key_is_pressed(GLFW_KEY_L)){
                _rotation.y -= forceRot;
                rotBleu += coeffRot;
                speed -= 0.000001;
            }            

            if(speed > limit) {
                speed = limit;
            }

            if(speed < 0.0) {
                speed = 0.0;
            }

            if(speed > 0.0) {
                speed -= 0.00001;
            }

            this->_position = {glm::cos(_rotation.y) * (speed * -1.0f) + _position.x, 0, glm::sin(_rotation.y) * (speed * -1.0f) + _position.z};
        }

        void draw(unsigned int modelLoc) {
        
            glm::mat4 base = glm::mat4(1.0);
            
            base = glm::translate(base, glm::vec3(_position.x, 0.0, _position.z));
            base = glm::rotate(base, -_rotation.y, glm::vec3(0.0f, 1.0f, 0.0f));
            base = glm::rotate(base, p6::PI / 2.f, glm::vec3(0.0f, 1.0f, 0.0f));
            base = glm::scale(base, glm::vec3(0.03)); // final scale
            // base = glm::translate(base, glm::vec3(0.0, 1.0, 0.0)); // fix final z
            
            glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(base));
            main.draw();
            
            glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(glm::rotate(base, -rotBleu, glm::vec3(1.0f, 0.0f, 0.0f))));
            rouge.draw();

            glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(glm::rotate(base, -rotRouge, glm::vec3(1.0f, 0.0f, 0.0f))));
            bleu.draw();

        }

        void drawMain() {
            
        }

        glm::vec3 getPos() {
            return _position;
        }

        glm::vec3 getRot() {
            return _rotation;
        }
};