#include "glm/ext/matrix_float4x4.hpp"
#include "glm/ext/matrix_transform.hpp"
#include "glm/ext/vector_float3.hpp"
#include "glm/fwd.hpp"
#include "glm/glm.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "glm/gtx/rotate_vector.hpp"

#include <cmath>

#include "glm/gtx/transform.hpp"
#include "p6/p6.h"

#include "glimac/default_shader.hpp"

#include "OpenGlWrapper.h"
#include "Vertex3D.h"

#include "camera.h"

#include "mesh.h"
#include "utils.h"

#include "./loaderGLTF/Model.h"

//https://opengl.developpez.com/tutoriels/apprendre-opengl/?page=systemes-de-coordonnees

int main() {
    auto ctx = p6::Context { { 1280, 720, "Project Texture" } };

    // ctx.maximize_window();

    Camera camera = Camera();
    camera.init();

    OpenGlWrapper cubeMap;
    initCubeMap(cubeMap, glm::vec3(0));

    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

    // glEnable(GL_CULL_FACE);
    // glCullFace(GL_FRONT);
    // glFrontFace(GL_CW);

    glEnable(GL_DEPTH_TEST);

    glm::mat4 projection = glm::perspective(glm::radians(45.0f), static_cast<float>(1280) / static_cast<float>(720), 0.001f, 100.0f);

    const p6::Shader cam = p6::load_shader(
        "shaders/camera.vs.glsl",
        "shaders/camera.fs.glsl");
    const p6::Shader skybox = p6::load_shader(
        "shaders/skybox.vs.glsl",
        "shaders/skybox.fs.glsl");
    const p6::Shader shaderCube = p6::load_shader(
        "shaders/cube.vs.glsl",
        "shaders/cube.fs.glsl");

    cam.use();
    cam.set("projection", projection);

    skybox.use();
    skybox.set("projection", projection);

    GLuint textureUnit = 0;

    skybox.use();
    std::vector<std::filesystem::path> faces = { "assets/cubemap/right.png",
                                                 "assets/cubemap/left.png",
                                                 "assets/cubemap/top.png",
                                                 "assets/cubemap/bottom.png",
                                                 "assets/cubemap/back.png",
                                                 "assets/cubemap/front.png" };

    loadAndBindCubemap(skybox, faces, "skybox", textureUnit);

    glActiveTexture(GL_TEXTURE0 + textureUnit + 1); // ?

    shaderCube.use();
    Model mod("./assets/models/castle/centre/centre.gltf");
    shaderCube.set("projection", projection);

    // glActiveTexture(GL_TEXTURE1);

    // Declare your infinite update loop.
    ctx.update
        = [&]() {
              /*********************************
         * HERE SHOULD COME THE RENDERING CODE
         *********************************/

              glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

              camera.update(ctx);
              glm::vec3 posCam = camera.getPos();

              // i -= 0.01;

              glm::mat4 model = glm::rotate(glm::mat4(1.0f), glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
              glm::mat4 view = glm::lookAt(posCam, glm::vec3(0), { 0, 1, 0 });

              cam.set("model", model);
              cam.set("view", view);
              skybox.set("view", view);

              skybox.use();
              cubeMap.draw();

              shaderCube.use();
              glm::mat4 vmodel = glm::scale(glm::mat4(1.0), glm::vec3(0.1));
              vmodel = glm::rotate(vmodel, p6::PI, glm::vec3(1.0f, 0.0f, 0.0f));
              //   vmodel = glm::translate(vmodel, glm::vec3(0.0, 10.0, 0.0));
              shaderCube.set("vmodel", vmodel);
              shaderCube.set("view", view);
              mod.Draw(shaderCube.id());
          };

    // Should be done last. It starts the infinite loop.
    ctx.start();
}