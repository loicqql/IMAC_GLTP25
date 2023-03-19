#pragma once

#include "glm/fwd.hpp"

#include "OpenGlWrapper.h"

class Boat {
    private : 
        glm::vec3 _position{};
        glm::vec3 _rotation{};

        OpenGlWrapper _openGlWrapper;
    
    public: 

        void init() {

            uint32_t indice = 0;

            _openGlWrapper._vertices.push_back(Vertex3D{{0, 0, 0.1}, {1.0, 0, 0}});
            _openGlWrapper._vertices.push_back(Vertex3D{{0 + 0.02, 0, 0.1}, {1.0, 0, 0}});
            _openGlWrapper._vertices.push_back(Vertex3D{{0 + 0.02, 0 + 0.02, 0.1}, {1.0, 0, 0}});
            _openGlWrapper._vertices.push_back(Vertex3D{{0, 0 + 0.02, 0.1}, {1.0, 0, 0}});

            _openGlWrapper._indices.push_back(indice);
            _openGlWrapper._indices.push_back(indice + 1);
            _openGlWrapper._indices.push_back(indice + 2);
            _openGlWrapper._indices.push_back(indice);
            _openGlWrapper._indices.push_back(indice + 2);
            _openGlWrapper._indices.push_back(indice + 3);
            indice += 4;

            _openGlWrapper.updateVertices();
            _openGlWrapper.updateIndices();

        }

        void update() {
            
        }

        void draw() const {
            _openGlWrapper.draw();
        }
};