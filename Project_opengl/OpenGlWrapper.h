#pragma once

#include <utility>
#include <vector>

#include "Vertex3D.h"

#include "glm/ext/vector_float2.hpp"
#include "glm/fwd.hpp"
#include "p6/p6.h"

struct TextureCube {
    glm::vec2 texCoord;
    glm::vec2 texId;
};

struct OpenGlWrapper {
    GLuint _vbo {};
    GLuint _vao {};
    GLuint _tex {};
    GLuint _nor {};

    std::vector<Vertex3D> _vertices;
    std::vector<uint32_t> _indices;
    std::vector<TextureCube> _textures;
    std::vector<glm::vec3> _normals;

    int _indice = 0;

    OpenGlWrapper() {

        glGenVertexArrays(1, &_vao);
        glBindVertexArray(_vao);

        glGenBuffers(1, &_vbo);

        glBindBuffer(GL_ARRAY_BUFFER, _vbo);
        glEnableVertexAttribArray(0);
        glEnableVertexAttribArray(1);

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex3D), (void*)offsetof(Vertex3D, position));
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex3D), (void*)offsetof(Vertex3D, color));

        glBindBuffer(GL_ARRAY_BUFFER, 0);

        // tex

        glGenBuffers(1, &_tex);

        glBindBuffer(GL_ARRAY_BUFFER, _tex);
        glEnableVertexAttribArray(2);
        glEnableVertexAttribArray(3);

        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(TextureCube), (void*)offsetof(TextureCube, texCoord));
        glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, sizeof(TextureCube), (void*)offsetof(TextureCube, texId));

        glBindBuffer(GL_ARRAY_BUFFER, 0);

        // normals

        glGenBuffers(1, &_nor);

        glBindBuffer(GL_ARRAY_BUFFER, _nor);
        glEnableVertexAttribArray(4);

        glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), nullptr);

        glBindBuffer(GL_ARRAY_BUFFER, 0);

        glBindVertexArray(0);
    }

    void updateVertices() {

        if (!_vertices.empty() && !_indices.empty()) {
            std::vector<Vertex3D> vertex;
            for (int i : _indices) {
                vertex.push_back(_vertices[i]);
            }

            glBindBuffer(GL_ARRAY_BUFFER, _vbo);
            glBufferData(GL_ARRAY_BUFFER, vertex.size() * sizeof(Vertex3D), &vertex.front(), GL_STATIC_DRAW);
            glBindBuffer(GL_ARRAY_BUFFER, 0);
        } else {
            std::string message;
            message += "Vertices array is empty";
            std::cerr << message << '\n';
            throw std::runtime_error { message };
        }
    }

    void updateTextures() {

        if (!_textures.empty()) {
            glBindBuffer(GL_ARRAY_BUFFER, _tex);
            glBufferData(GL_ARRAY_BUFFER, _textures.size() * sizeof(TextureCube), &_textures.front(), GL_STATIC_DRAW);
            glBindBuffer(GL_ARRAY_BUFFER, 0);
        } else {
            std::string message;
            message += "Textures array is empty";
            std::cerr << message << '\n';
            throw std::runtime_error { message };
        }
    }

    void updateNormals() {

        if (!_normals.empty()) {
            glBindBuffer(GL_ARRAY_BUFFER, _nor);
            glBufferData(GL_ARRAY_BUFFER, _normals.size() * sizeof(glm::vec3), &_normals.front(), GL_STATIC_DRAW);
            glBindBuffer(GL_ARRAY_BUFFER, 0);
        } else {
            std::string message;
            message += "Normals array is empty";
            std::cerr << message << '\n';
            throw std::runtime_error { message };
        }
    }

    void draw() const {

        if (!_vertices.empty() && !_indices.empty()) {

            glBindVertexArray(_vao);
            glDrawArrays(GL_TRIANGLES, 0, _indices.size());
            glBindVertexArray(0);
        }
    }

    //destructor
};