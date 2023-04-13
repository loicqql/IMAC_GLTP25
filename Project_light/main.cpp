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

#include "shadow_map_fbo.h"

#include <fstream>

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

    ShadowMapFBO shadowMapFBO;
    shadowMapFBO.Init(1280, 720);

    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    // glFrontFace(GL_CW);
    // glCullFace(GL_BACK);
    // glEnable(GL_CULL_FACE);
    glEnable(GL_DEPTH_TEST);

    glm::mat4 projection = glm::perspective(glm::radians(45.0f), static_cast<float>(1280) / static_cast<float>(720), 0.001f, 100.0f);

    const p6::Shader shader = p6::load_shader(
        "shaders/shadow_render.vs.glsl",
        "shaders/shadow_render.fs.glsl"
    );

    const p6::Shader shaderSh = p6::load_shader(
        "shaders/shadow_map.vs.glsl",
        "shaders/shadow_map.fs.glsl"
    );

    const p6::Shader cam = p6::load_shader(
        "shaders/camera.vs.glsl",
        "shaders/camera.fs.glsl"
    );

    shader.set("projection", projection);
    shaderSh.set("projection", projection);
    cam.set("projection", projection);

    // Declare your infinite update loop.
    ctx.update = [&]() {
        /*********************************
         * HERE SHOULD COME THE RENDERING CODE
         *********************************/

        camera.update(ctx);
        glm::vec3 posCam = camera.getPos();

        // i -= 0.01;

        //shadow pass pass        

        shadowMapFBO.BindForWriting();
        glClear(GL_DEPTH_BUFFER_BIT);
        glCullFace(GL_BACK);

        shaderSh.use();

        glm::mat4 model = glm::rotate(glm::mat4(1.0f), glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
        glm::mat4 view = glm::lookAt({0.25, 0.5, 0.25}, glm::vec3(0), {0 , 1, 0});
        // glm::mat4 view = glm::lookAt(posCam, glm::vec3(0), {0 , 1, 0});

        projection = glm::ortho<float>(-1,1,-1,1,-1,10);

        shaderSh.set("projection", projection);
        shaderSh.set("model", model);
        shaderSh.set("view", view);

        plane.draw();
        cube.draw();
        

        glm::mat4 DepthMVP = (projection * view * model);

        //render pass

        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        glViewport(0, 0, 1280, 720);

        shader.use();

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glCullFace(GL_FRONT);
      
        glUniform1i(glGetUniformLocation(shader.id(), "gShadowMap"), 0);
        shadowMapFBO.BindForReading(GL_TEXTURE0);

        model = glm::rotate(glm::mat4(1.0f), glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
        view = glm::lookAt(posCam, glm::vec3(0), {0 , 1, 0});

        shader.set("model", model);
        shader.set("view", view);

        shader.set("DepthMVP", DepthMVP);

        plane.draw();
        cube.draw();

    };

    // Should be done last. It starts the infinite loop.
    ctx.start();
    
}