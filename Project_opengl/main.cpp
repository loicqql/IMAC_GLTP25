#include "glm/ext/matrix_transform.hpp"
#include "glm/ext/vector_float3.hpp"
#include "glm/glm.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "glm/gtx/rotate_vector.hpp"

#include "./OpenGlWrapper.h"
#include "./skybox.h"
#include "./utils.h"

#include <cmath>

#include "glm/gtx/transform.hpp"
#include "p6/p6.h"

#include "glimac/default_shader.hpp"

#include "Boat/Boat.h"
#include "Boid/Boid.h"
#include "Camera/Camera.h"
#include "Terrain/Terrain.h"
#include "Vertex3D.h"

#include "Shadow/ShadowFrameBuffers.h"
#include "Water/WaterFrameBuffers.h"

// #include "./loaders/gltf/loaderGLTF.h"
#include "./loaderGLTF/Model.h"

//https://opengl.developpez.com/tutoriels/apprendre-opengl/?page=systemes-de-coordonnees

int main() {
    auto ctx = p6::Context { { 1280, 720, "Project Ground" } };
    // ctx.maximize_window();

    std::random_device rd;
    std::mt19937 mt(rd());
    std::uniform_real_distribution<double> randPosition(-1.0, 1.0);
    std::uniform_real_distribution<double> randVelocity(-0.001, 0.001);

    // std::vector<Boid> boids;

    // for (int i = 0; i < 50; ++i) {
    //     Boid boid(glm::vec3(randPosition(mt), randPosition(mt), 0.5), glm::vec3(randVelocity(mt), randVelocity(mt), 0));
    //     boids.emplace_back(boid);
    //     boids[i].init();
    // }

    Camera camera;
    Boat boat;
    Terrain terrain;
    WaterFrameBuffers waterfbos;
    ShadowFrameBuffers shadowMap;

    OpenGlWrapper skybox;
    initSkybox(skybox);

    // ShadersManager shadersManager;

    const float WAVE_SPEED = 0.05f;
    float moveWater = 0;

    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CLIP_DISTANCE0);

    glEnable(GL_CULL_FACE);
    glCullFace(GL_FRONT);
    glFrontFace(GL_CW);

    const p6::Shader shaderOr = p6::load_shader(
        "shaders/terrain.vs.glsl",
        "shaders/terrainOr.fs.glsl");

    const p6::Shader shaderAr = p6::load_shader(
        "shaders/terrain.vs.glsl",
        "shaders/terrainAr.fs.glsl");

    const p6::Shader shaderSmall = p6::load_shader(
        "shaders/terrain.vs.glsl",
        "shaders/terrainSmall.fs.glsl");

    const p6::Shader shaderCube = p6::load_shader(
        "shaders/cube.vs.glsl",
        "shaders/cube.fs.glsl");

    const p6::Shader shaderWater = p6::load_shader(
        "shaders/water.vs.glsl",
        "shaders/water.fs.glsl");

    const p6::Shader shaderShadowGen = p6::load_shader(
        "shaders/shadow_gen.vs.glsl",
        "shaders/shadow_gen.fs.glsl");

    const p6::Shader shaderSkybox = p6::load_shader(
        "shaders/skybox.vs.glsl",
        "shaders/skybox.fs.glsl");

    const p6::Shader shaderSimple = p6::load_shader(
        "shaders/simple.vs.glsl",
        "shaders/simple.fs.glsl");

    const p6::Shader shaderGLTF = p6::load_shader(
        "shaders/gltf.vs.glsl",
        "shaders/gltf.fs.glsl");

    auto setAllUniform = [&](auto uniform_name, auto value) {
        shaderOr.set(uniform_name, value);
        shaderAr.set(uniform_name, value);
        shaderSmall.set(uniform_name, value);
        shaderCube.set(uniform_name, value);
        shaderWater.set(uniform_name, value);
        shaderShadowGen.set(uniform_name, value);
        shaderSkybox.set(uniform_name, value);
        shaderSimple.set(uniform_name, value);
        shaderGLTF.set(uniform_name, value);
    };

    glm::mat4 projection = glm::mat4(1.0f);
    glm::mat4 view = glm::mat4(1.0f);
    glm::mat4 model = glm::mat4(1.0f);

    projection = glm::perspective(glm::radians(45.0f), static_cast<float>(1280) / static_cast<float>(720), 0.001f, 100.0f);
    model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));

    setAllUniform("projection", projection);
    setAllUniform("model", model);

    // glEnable(GL_BLEND);
    // glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    // glBlendColor(1.0f, 1.0f, 1.0f, 1.0f);

    glm::vec3 sunColor = { 1.0, 1.0, 1.0 };
    glm::vec3 sunPosition = { -0.5, 0.5, -0.5 };

    GLuint textureUnit = 0;

    shaderWater.use();
    bindTexture(shaderWater, waterfbos.getReflectionTexture(), "reflectionTexture", textureUnit);
    bindTexture(shaderWater, waterfbos.getRefractionTexture(), "refractionTexture", textureUnit);
    loadAndBindTexture(shaderWater, "assets/waterDUDV.png", "dudvMap", textureUnit);
    loadAndBindTexture(shaderWater, "assets/waterNormalMap.png", "normalMap", textureUnit);

    shaderOr.use();
    loadAndBindTexture(shaderOr, "assets/textures/or1grain.png", "or1", textureUnit);
    loadAndBindTexture(shaderOr, "assets/textures/or2grain.png", "or2", textureUnit);
    loadAndBindTexture(shaderOr, "assets/textures/or3grain.png", "or3", textureUnit);
    bindTexture(shaderOr, shadowMap.getShadowTexture(), "gShadowMap", textureUnit);

    shaderAr.use();
    loadAndBindTexture(shaderAr, "assets/textures/ar1grain.png", "ar1", textureUnit);
    loadAndBindTexture(shaderAr, "assets/textures/ar2grain.png", "ar2", textureUnit);
    loadAndBindTexture(shaderAr, "assets/textures/ar3grain.png", "ar3", textureUnit);
    bindTexture(shaderAr, shadowMap.getShadowTexture(), "gShadowMap", textureUnit);

    shaderSmall.use();
    loadAndBindTexture(shaderSmall, "assets/textures/smallgrain.png", "small", textureUnit);
    bindTexture(shaderSmall, shadowMap.getShadowTexture(), "gShadowMap", textureUnit);

    shaderGLTF.use();
    bindTexture(shaderGLTF, shadowMap.getShadowTexture(), "gShadowMap", textureUnit);

    shaderSkybox.use();
    std::vector<std::filesystem::path> faces = { "assets/cubemap/right.png",
                                                 "assets/cubemap/left.png",
                                                 "assets/cubemap/top.png",
                                                 "assets/cubemap/bottom.png",
                                                 "assets/cubemap/back.png",
                                                 "assets/cubemap/front.png" };

    loadAndBindCubemap(shaderSkybox, faces, "skybox", textureUnit);

    glm::mat4 shadowProj = glm::ortho<float>(-2.5, 2.5, -2.5, 2.5, -2.5, 2.5);

    loaderGLTF castle;
    shaderCube.use();
    castle.load("./assets/models/castle/untitled.gltf", shaderCube.id(), textureUnit);

    glActiveTexture(GL_TEXTURE0 + textureUnit + 1); // ?

    shaderGLTF.use();
    Model centre("./assets/models/castle/centre/centre.gltf");
    shaderGLTF.set("projection", projection);

    // Declare your infinite update loop.
    ctx.update = [&]() {
        const float OCEAN_HEIGHT = 0.f;
        const glm::vec3 offsetCenterCamera = glm::vec3(0, 0.04, 0);

        moveWater += WAVE_SPEED * ctx.delta_time();

        boat.update(ctx);
        camera.update(ctx, boat);
        glm::vec3 posCam = camera.getPos();

        //ENV
        glm::mat4 modelC = glm::mat4(1.0);
        modelC = glm::translate(modelC, glm::vec3(-0.5, -0.01, 0.85));
        modelC = glm::rotate(modelC, p6::PI, glm::vec3(1.0f, 0.0f, 0.0f));
        modelC = glm::rotate(modelC, -p6::PI / 2.f, glm::vec3(0.0f, 1.0f, 0.0f));
        modelC = glm::scale(modelC, glm::vec3(0.012));

        // ---------------------------------
        // RENDER SHADOWS
        // ---------------------------------
        shadowMap.BindForWriting();
        glClear(GL_DEPTH_BUFFER_BIT);

        shaderShadowGen.use();

        glm::mat4 shadowView = glm::lookAt(sunPosition, glm::vec3(0), { 0, 1, 0 });

        shaderShadowGen.set("projection", shadowProj);
        shaderShadowGen.set("view", shadowView);

        terrain.drawTerrain();

        shaderGLTF.use();
        shaderGLTF.set("model", modelC);
        centre.Draw(shaderGLTF.id());

        glm::mat4 DepthMVP = shadowProj * shadowView * model;

        setAllUniform("DepthMVP", DepthMVP);

        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        glViewport(0, 0, 1280, 720);

        // ---------------------------------
        // RENDER REFLECTION TEXTURE
        // ---------------------------------
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glm::vec3 posCamReflection = posCam;
        float distance = 2 * (posCam.y - OCEAN_HEIGHT);
        posCamReflection.y -= distance;
        setAllUniform("view", glm::lookAt(posCamReflection, boat.getPos() - offsetCenterCamera, { 0, 1, 0 }));
        setAllUniform("plane", glm::vec4(0, 1, 0, -OCEAN_HEIGHT));
        waterfbos.bindReflectionFrameBuffer();

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        shaderOr.use();
        terrain.drawMountainOr();

        shaderAr.use();
        terrain.drawMountainAr();

        shaderSmall.use();
        terrain.drawMountainSmall();

        shaderSkybox.use();
        skybox.draw();

        /*
        shaderCube.use();
        boat.draw(shaderCube); // test w/ final model
        */

        shaderGLTF.use();
        shaderGLTF.set("model", modelC);
        centre.Draw(shaderGLTF.id());

        waterfbos.unbindCurrentFrameBuffer();

        view = glm::lookAt(posCam, boat.getPos() + offsetCenterCamera, { 0, 1, 0 });
        setAllUniform("view", view);

        // ---------------------------------
        // RENDER REFRACTION TEXTURE
        // ---------------------------------
        setAllUniform("plane", glm::vec4(0, -1, 0, OCEAN_HEIGHT));
        waterfbos.bindRefractionFrameBuffer();
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        shaderOr.use();
        terrain.drawMountainOr();

        shaderAr.use();
        terrain.drawMountainAr();

        shaderSmall.use();
        terrain.drawMountainSmall();

        shaderSkybox.use();
        skybox.draw();

        shaderCube.use();
        boat.draw(shaderCube);
        waterfbos.unbindCurrentFrameBuffer();

        shaderGLTF.use();
        shaderGLTF.set("model", modelC);
        centre.Draw(shaderGLTF.id());

        // ---------------------------------
        // RENDER TO SCREEN
        // ---------------------------------
        setAllUniform("plane", glm::vec4(0, -1, 0, 100));

        shaderOr.use();
        terrain.drawMountainOr();

        shaderAr.use();
        terrain.drawMountainAr();

        shaderSmall.use();
        terrain.drawMountainSmall();

        shaderSkybox.use();
        skybox.draw();

        shaderWater.use();
        shaderWater.set("lightPosition", sunPosition);
        shaderWater.set("lightColor", sunColor);
        shaderWater.set("moveWater", moveWater);
        shaderWater.set("cameraPosition", posCam);
        terrain.drawWater();

        // shaderCube.use();
        // boat.draw(shaderCube);

        // shaderCube.use();
        // glm::mat4 castleModel = glm::mat4(1.0);
        // castleModel = glm::translate(castleModel, glm::vec3(0.0, 0.5, 0.0));
        // castleModel = glm::scale(castleModel, glm::vec3(0.2));
        // shaderCube.set("model", castleModel);
        // castle.draw();

        shaderGLTF.use();
        shaderGLTF.set("model", modelC);
        centre.Draw(shaderGLTF.id());

        // for (uint i = 0; i < boids.size(); ++i) {
        //     boids[i].update(ctx, boids);
        //     boids[i].draw(glGetUniformLocation(shaderCube.id(), "model"));
        // }
    };

    // Should be done last. It starts the infinite loop.
    ctx.start();

    // glDeleteBuffers(1, &vbo);
    // glDeleteVertexArrays(1, &vao);
}