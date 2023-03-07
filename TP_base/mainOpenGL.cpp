#include "p6/p6.h"

#include "glimac/default_shader.hpp"

int main()
{
    auto ctx = p6::Context{{1280, 720, "TP"}};
    // ctx.maximize_window();

    /*********************************
     * HERE SHOULD COME THE INITIALIZATION CODE
     *********************************/

    struct Vertex3D {
        glm::vec3 position;
        // glm::vec3 color;
    };

    /* code */
    //vbo
    GLuint vbo;
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);

    std::vector<Vertex3D> vertices;

    vertices.push_back(Vertex3D{{-0.5, -0.5, 0.0}});
    vertices.push_back(Vertex3D{{0.5, -0.5, 0.0}});
    vertices.push_back(Vertex3D{{0.0, 0.5, 0.0}});


    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex3D), &vertices.front(), GL_STATIC_DRAW); 

    glBindBuffer(GL_ARRAY_BUFFER, 0);

    //vao
    GLuint vao;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao); 

    glBindBuffer(GL_ARRAY_BUFFER, vbo); 

    glEnableVertexAttribArray(0);
    // glEnableVertexAttribArray(8);   
    
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex3D), (void*) offsetof(Vertex3D, position));
    // glVertexAttribPointer(8, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex3D), (void*) offsetof(Vertex3D, color));
    
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
        

        glDrawArrays(GL_TRIANGLES, 0, vertices.size());

        glBindVertexArray(0);

    };

    // Should be done last. It starts the infinite loop.
    ctx.start();

    glDeleteBuffers(1, &vbo);
    glDeleteVertexArrays(1, &vao);
    
}