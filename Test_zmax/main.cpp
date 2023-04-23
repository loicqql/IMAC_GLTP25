#include "glm/ext/matrix_transform.hpp"
#include "glm/ext/vector_float3.hpp"
#include "glm/fwd.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "glm/gtx/rotate_vector.hpp"
#include "glm/glm.hpp"

#include <cmath>

#include "p6/p6.h"

#include "glimac/default_shader.hpp"

#include "Vertex3D.h"
#include "OpenGlWrapper.h"

#include "camera.h"

#include "mesh.h"

//https://opengl.developpez.com/tutoriels/apprendre-opengl/?page=systemes-de-coordonnees

int main()
{
    auto ctx = p6::Context{{1280, 720, "Project Light"}};
    
    // ctx.maximize_window();

    Camera camera = Camera();
    camera.init();

    OpenGlWrapper plane;
    initPlane(plane, glm::vec3(0.5f));

    OpenGlWrapper cube;
    initCube(cube, glm::vec3(0.7f));

    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

    glEnable(GL_CULL_FACE);
    glCullFace(GL_FRONT);
    glFrontFace(GL_CW);

    glEnable(GL_DEPTH_TEST);

    glm::mat4 projection = glm::perspective(glm::radians(45.0f), static_cast<float>(1280) / static_cast<float>(720), 0.001f, 100.0f);

    const p6::Shader cam = p6::load_shader(
        "shaders/camera.vs.glsl",
        "shaders/camera.fs.glsl"
    );

    cam.use();
    cam.set("projection", projection);

    // Declare your infinite update loop.
    ctx.update = [&]() {
        /*********************************
         * HERE SHOULD COME THE RENDERING CODE
         *********************************/

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        camera.update(ctx);
        glm::vec3 posCam = camera.getPos();

        // i -= 0.01;        

        glm::mat4 model = glm::rotate(glm::mat4(1.0f), glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
        glm::mat4 view = glm::lookAt(posCam, glm::vec3(0), {0 , 1, 0});

        cam.set("model", model);
        cam.set("view", view);

        plane.draw();
        cube.draw();

    };

    // Should be done last. It starts the infinite loop.
    ctx.start();
    
}