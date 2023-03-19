#pragma once

#include "GLFW/glfw3.h"
#include "glm/fwd.hpp"

#include "OpenGlWrapper.h"

#include "p6/p6.h"

class Boat {
    private : 
        glm::vec3 _position{};
        glm::vec3 _rotation{};

        OpenGlWrapper _openGlWrapper;

        float width = 0.02;
        float speed = 0.0;
    
    public: 

        void init() {

            _position = {0.5, 0, 0.1};
            _rotation = {0, 0, 0};


            updateVertices();

            uint32_t indice = 0;

            _openGlWrapper._indices.push_back(indice);
            _openGlWrapper._indices.push_back(indice + 1);
            _openGlWrapper._indices.push_back(indice + 2);
            _openGlWrapper._indices.push_back(indice);
            _openGlWrapper._indices.push_back(indice + 2);
            _openGlWrapper._indices.push_back(indice + 3);
            indice += 4;

            
            _openGlWrapper.updateIndices();

        }

        void update(p6::Context &ctx) {

            float forceRot = 0.01;
            float limit = 0.0017; 

            if(ctx.key_is_pressed(GLFW_KEY_S) && ctx.key_is_pressed(GLFW_KEY_L)) {
                speed += 0.00005;
            }else if(ctx.key_is_pressed(GLFW_KEY_S)) {
                _rotation.y -= forceRot;
                speed -= 0.000001;
            }else if(ctx.key_is_pressed(GLFW_KEY_L)){
                _rotation.y += forceRot;
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

            this->_position = {glm::cos(_rotation.y + p6::PI) * speed + _position.x, 0, glm::sin(_rotation.y + p6::PI) * speed + _position.z};

            updateVertices();
        }

        void draw() const {
            _openGlWrapper.draw();
        }

        glm::vec3 getPos() {
            return _position;
        }

        glm::vec3 getRot() {
            return _rotation;
        }

    private: 

        void updateVertices() {
            _openGlWrapper._vertices.clear();

            _openGlWrapper._vertices.push_back(Vertex3D{{_position.x, _position.z * -1.0, 0.04}, {1.0, 0, 0}});
            _openGlWrapper._vertices.push_back(Vertex3D{{_position.x + width, _position.z * -1.0, 0.04}, {1.0, 0, 0}});
            _openGlWrapper._vertices.push_back(Vertex3D{{_position.x + width, (_position.z + width) * -1.0, 0.04}, {1.0, 0, 0}});
            _openGlWrapper._vertices.push_back(Vertex3D{{_position.x, (_position.z + width) * -1.0, 0.04}, {1.0, 0, 0}});

            _openGlWrapper.updateVertices();
        }
};