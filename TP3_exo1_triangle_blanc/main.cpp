#include "p6/p6.h"

#include "glimac/default_shader.hpp"

int main()
{
    auto ctx = p6::Context{{1280, 720, "TP3 EX1"}};
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
        -0.5f, -0.5f,
        0.5f, -0.5f,
        0.0f,  0.5f
    };

    glBufferData(GL_ARRAY_BUFFER, 6 * sizeof(GLfloat), vertices, GL_STATIC_DRAW); 

    glBindBuffer(GL_ARRAY_BUFFER, 0);

    //vao
    GLuint vao;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    static constexpr GLuint vertex_attr_position = 0;
    glEnableVertexAttribArray(vertex_attr_position);    

    glBindBuffer(GL_ARRAY_BUFFER, vbo);    
    
    glVertexAttribPointer(
        vertex_attr_position, 
        2, 
        GL_FLOAT, 
        GL_FALSE, 
        2 * sizeof(GLfloat), 
        0
    );

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    
    glBindVertexArray(0);

    // Declare your infinite update loop.
    ctx.update = [&]() {
        /*********************************
         * HERE SHOULD COME THE RENDERING CODE
         *********************************/

        glClear(GL_COLOR_BUFFER_BIT);

        glBindVertexArray(vao);

        glimac::bind_default_shader();

        glDrawArrays(GL_TRIANGLES, 0, 3);

        glBindVertexArray(0);

    };

    // Should be done last. It starts the infinite loop.
    ctx.start();

    glDeleteBuffers(1, &vbo);
    glDeleteVertexArrays(1, &vao);
    
}