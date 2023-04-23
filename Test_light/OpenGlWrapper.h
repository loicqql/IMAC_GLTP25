#pragma once

#include <utility>
#include <vector>

#include "Vertex3D.h"

#include "p6/p6.h"


struct OpenGlWrapper {
    GLuint _vbo{};
    GLuint _vao{};
    GLuint _ibo{};

    std::vector<Vertex3D> _vertices;
    std::vector<uint32_t> _indices;

    OpenGlWrapper() {
        glGenBuffers(1, &_vbo);

        glGenBuffers(1, &_ibo);
        
        glGenVertexArrays(1, &_vao);

        glBindVertexArray(_vao);

        glBindBuffer(GL_ARRAY_BUFFER, _vbo);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _ibo);
        glEnableVertexAttribArray(0);
        glEnableVertexAttribArray(1);

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex3D), (void*) offsetof(Vertex3D, position));
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex3D), (void*) offsetof(Vertex3D, color));
        
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        
        glBindVertexArray(0);
    }

    void updateVertices() {

        if(!_vertices.empty()) {
            glBindBuffer(GL_ARRAY_BUFFER, _vbo);
            glBufferData(GL_ARRAY_BUFFER, _vertices.size() * sizeof(Vertex3D), &_vertices.front(), GL_STATIC_DRAW); 
            glBindBuffer(GL_ARRAY_BUFFER, 0);
        }else {
            std::string message;
            message += "Vertices array is empty";
            std::cerr << message << '\n';
            throw std::runtime_error{message};
        }

    }

    void updateIndices() {

        if(!_indices.empty())  {
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _ibo);
            glBufferData(GL_ELEMENT_ARRAY_BUFFER, _indices.size() * sizeof(uint32_t), &_indices.front(), GL_STATIC_DRAW);
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
        }else {
            std::string message;
            message += "Indices array is empty";
            std::cerr << message << '\n';
            throw std::runtime_error{message};
        }

    }

    void draw() const {

        if(!_vertices.empty() && !_indices.empty()) {

            glBindVertexArray(_vao);
            glDrawElements(GL_TRIANGLES, _indices.size(), GL_UNSIGNED_INT, nullptr);
            glBindVertexArray(0);
            
        }

    }

    //destructor

};