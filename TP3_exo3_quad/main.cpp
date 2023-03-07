#include "p6/p6.h"
#include "glm/glm.hpp"


int main()
{
    auto ctx = p6::Context{{1280, 720, "TP3 EX4"}};
    // ctx.maximize_window();

    /*********************************
     * HERE SHOULD COME THE INITIALIZATION CODE
     *********************************/

    struct Vertex2DColor {
        glm::vec2 position;
        glm::vec3 color;
    };

    /* code */
    //vbo
    GLuint vbo;
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);

    Vertex2DColor vertices[] = { 
        Vertex2DColor{{-0.5f, -0.5f}, {1.f, 0.f, 0.f}}, // Sommet 0
        Vertex2DColor{{+0.5f, -0.5f}, {0.f, 1.f, 0.f}}, // Sommet 1
        Vertex2DColor{{+0.5f, +0.5f}, {0.f, 0.f, 1.f}}, // Sommet 2
        Vertex2DColor{{-0.5f, +0.5f}, {1.f, 1.f, 1.f}}, // Sommet 3
    };

    glBufferData(GL_ARRAY_BUFFER, 4 * sizeof(Vertex2DColor), vertices, GL_STATIC_DRAW); 

    glBindBuffer(GL_ARRAY_BUFFER, 0);

    GLuint ibo;
    glGenBuffers(1, &ibo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);

    uint32_t indices[] = {
        0, 1, 2,
        0, 2, 3
    };

    glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6 * sizeof(uint32_t), indices, GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    //vao
    GLuint vao;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao); 

    glBindBuffer(GL_ARRAY_BUFFER, vbo);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo); 

    glEnableVertexAttribArray(3);
    glEnableVertexAttribArray(8);   
    
    glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex2DColor), (void*) offsetof(Vertex2DColor, position));
    glVertexAttribPointer(8, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex2DColor), (void*) offsetof(Vertex2DColor, color));
    
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    
    glBindVertexArray(0);

    const p6::Shader shader = p6::load_shader(
        "shaders/triangle.vs.glsl",
        "shaders/triangle.fs.glsl"
    );

    // Declare your infinite update loop.
    ctx.update = [&]() {
        /*********************************
         * HERE SHOULD COME THE RENDERING CODE
         *********************************/

        glClear(GL_COLOR_BUFFER_BIT);

        glBindVertexArray(vao);

        shader.use();

        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        glBindVertexArray(0);

    };

    // Should be done last. It starts the infinite loop.
    ctx.start();

    glDeleteBuffers(1, &vbo);
    glDeleteVertexArrays(1, &vao);
    
}