#include "p6/p6.h"

#include "glimac/default_shader.hpp"

int main()
{
    auto ctx = p6::Context{{1280, 720, "TP3 EX2"}};
    // ctx.maximize_window();

    /*********************************
     * HERE SHOULD COME THE INITIALIZATION CODE
     *********************************/

    /* code */
    //vbo
    GLuint vbo;
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);

    GLfloat vertices[] = { 
        -0.5f, -0.5f, 1.f, 0.f, 0.f, // Premier sommet
        0.5f, -0.5f, 0.f, 1.f, 0.f, // Deuxième sommet
        0.0f,  0.5f, 0.f, 0.f, 1.f  // Troisième sommet
    };

    glBufferData(GL_ARRAY_BUFFER, 15 * sizeof(GLfloat), vertices, GL_STATIC_DRAW); 

    glBindBuffer(GL_ARRAY_BUFFER, 0);

    //vao
    GLuint vao;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao); 

    glBindBuffer(GL_ARRAY_BUFFER, vbo); 

    glEnableVertexAttribArray(3);
    glEnableVertexAttribArray(8);   
    
    glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), 0);
    glVertexAttribPointer(8, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (void*) (2 * sizeof(GLfloat)));
    
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

        glDrawArrays(GL_TRIANGLES, 0, 3);

        glBindVertexArray(0);

    };

    // Should be done last. It starts the infinite loop.
    ctx.start();

    glDeleteBuffers(1, &vbo);
    glDeleteVertexArrays(1, &vao);
    
}