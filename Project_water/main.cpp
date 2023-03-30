#include "glm/ext/matrix_transform.hpp"
#include "glm/ext/vector_float3.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "glm/gtx/rotate_vector.hpp"
#include "glm/glm.hpp"

#include <math.h>

#include "p6/p6.h"

#include "glimac/default_shader.hpp"

#include "Vertex3D.h"
#include "Boat.h"

//https://opengl.developpez.com/tutoriels/apprendre-opengl/?page=systemes-de-coordonnees

int main()
{
    auto ctx = p6::Context{{1280, 720, "Project Ground"}};
    // ctx.maximize_window();

    Boat boat;
    boat.init();

    glEnable(GL_DEPTH_TEST);

    glm::mat4 projection = glm::mat4(1.0f);
    projection = glm::perspective(glm::radians(45.0f), (float)ctx.current_canvas_width() / (float)ctx.current_canvas_height(), 0.1f, 100.0f);

    const p6::Shader shader = p6::load_shader(
        "shaders/test.vs.glsl",
        "shaders/test.fs.glsl"
    );

    shader.use(); 
    shader.set("projection", projection);


    // float i = 0.0;

    // Declare your infinite update loop.
    ctx.update = [&]() {
        /*********************************
         * HERE SHOULD COME THE RENDERING CODE
         *********************************/

        boat.update(ctx);

        // i -= 0.01;

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // glimac::bind_default_shader();

        shader.use();        

        glm::mat4 model = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
        glm::mat4 view = glm::mat4(1.0f);
        

        model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
        view  = glm::translate(view, glm::vec3(0.0f, -0.2f, -1.0f));
        // view = glm::lookAt(posCam, boat.getPos(), {0 , 1, 0});
        
        // retrieve the matrix uniform locations
        unsigned int modelLoc = glGetUniformLocation(shader.id(), "model");
        unsigned int viewLoc  = glGetUniformLocation(shader.id(), "view");
        // pass them to the shaders (3 different ways)
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, &view[0][0]);

        
        boat.draw();

    };

    // Should be done last. It starts the infinite loop.
    ctx.start();

    // glDeleteBuffers(1, &vbo);
    // glDeleteVertexArrays(1, &vao);
    
}