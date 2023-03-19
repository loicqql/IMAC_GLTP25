#include "glm/ext/matrix_transform.hpp"
#include "glm/ext/vector_float3.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "glm/gtx/rotate_vector.hpp"
#include "glm/glm.hpp"

#include <math.h>

#include "p6/p6.h"

#include "glimac/default_shader.hpp"

#include "Vertex3D.h"
#include "camera.h"
#include "Boat.h"
#include "Terrain.h"

//https://opengl.developpez.com/tutoriels/apprendre-opengl/?page=systemes-de-coordonnees

int main()
{
    auto ctx = p6::Context{{1280, 720, "Project Ground"}};
    // ctx.maximize_window();


    Camera camera = Camera();
    camera.init();

    Boat boat;
    boat.init();

    Terrain terrain;
    terrain.init();

    glEnable(GL_DEPTH_TEST);


    const p6::Shader shader = p6::load_shader(
        "shaders/camera.vs.glsl",
        "shaders/camera.fs.glsl"
    );


    float i = 0.0;

    // Declare your infinite update loop.
    ctx.update = [&]() {
        /*********************************
         * HERE SHOULD COME THE RENDERING CODE
         *********************************/

        camera.update(glm::vec3(0.0f, 0.f, 0.0f), glm::vec3(0.0f, i, 0.0f));
        glm::vec3 posCam = camera.getPos();

        i -= 0.01;

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        shader.use();        

        glm::mat4 model = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
        glm::mat4 view = glm::mat4(1.0f);
        glm::mat4 projection = glm::mat4(1.0f);

        model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
        // view  = glm::translate(view, glm::vec3(0.0f, 0, -2.0f));

        view = glm::lookAt(posCam, {0, 0, 0}, {0 , 1, 0});

        projection = glm::perspective(glm::radians(45.0f), (float)ctx.current_canvas_width() / (float)ctx.current_canvas_height(), 0.1f, 100.0f);
        // retrieve the matrix uniform locations
        unsigned int modelLoc = glGetUniformLocation(shader.id(), "model");
        unsigned int viewLoc  = glGetUniformLocation(shader.id(), "view");
        // pass them to the shaders (3 different ways)
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, &view[0][0]);
        // note: currently we set the projection matrix each frame, but since the projection matrix rarely changes it's often best practice to set it outside the main loop only once.
        shader.set("projection", projection);

        // glimac::bind_default_shader();

        terrain.draw();
        boat.draw();

    };

    // Should be done last. It starts the infinite loop.
    ctx.start();

    // glDeleteBuffers(1, &vbo);
    // glDeleteVertexArrays(1, &vao);
    
}