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
        Vertex2DColor{{-0.5f, -0.5f}, {1.f, 0.f, 0.f}},
        Vertex2DColor{{ 0.5f, -0.5f}, {0.f, 1.f, 0.f}},
        Vertex2DColor{{-0.5f,  0.5f}, {0.f, 0.f, 1.f}},
        //
        Vertex2DColor{{-0.5f,  0.5f}, {0.f, 0.f, 1.f}},
        Vertex2DColor{{0.5f,  0.5f}, {0.f, 0.f, 1.f}},
        Vertex2DColor{{0.5f,  -0.5f}, {0.f, 0.f, 1.f}},
    };

    glBufferData(GL_ARRAY_BUFFER, 6 * sizeof(Vertex2DColor), vertices, GL_STATIC_DRAW); 

    glBindBuffer(GL_ARRAY_BUFFER, 0);

    //vao
    GLuint vao;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao); 

    glBindBuffer(GL_ARRAY_BUFFER, vbo); 

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

        glDrawArrays(GL_TRIANGLES, 0, 6);

        glBindVertexArray(0);

    };

    // Should be done last. It starts the infinite loop.
    ctx.start();

    glDeleteBuffers(1, &vbo);
    glDeleteVertexArrays(1, &vao);
    
}