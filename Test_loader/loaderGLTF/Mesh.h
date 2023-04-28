
#pragma once

// #include "Camera.h"
#include "EBO.h"
#include "Texture.h"
#include "VAO.h"
#include "glm/glm.hpp"
#include "p6/p6.h"
#include <string>

#include "glm/ext/matrix_transform.hpp"
#include "glm/ext/vector_float3.hpp"
#include "glm/glm.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "glm/gtx/rotate_vector.hpp"

class Mesh {
public:
    std::vector<Vertex> vertices;
    std::vector<GLuint> indices;
    std::vector<Texture> textures;
    // Store VAO in public so it can be used in the Draw function
    VAO myVao;

    // Initializes the mesh
    Mesh(std::vector<Vertex>& vertices, std::vector<GLuint>& indices, std::vector<Texture>& textures);

    // Draws the mesh
    void Draw(
        GLuint shaderId,
        // Camera& camera,
        glm::mat4 matrix = glm::mat4(1.0f),
        glm::vec3 translation = glm::vec3(0.0f, 0.0f, 0.0f),
        glm::quat rotation = glm::quat(1.0f, 0.0f, 0.0f, 0.0f),
        glm::vec3 scale = glm::vec3(1.0f, 1.0f, 1.0f));
};