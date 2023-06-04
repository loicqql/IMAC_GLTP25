#include "glm/ext/matrix_transform.hpp"
#include "glm/ext/vector_float3.hpp"
#include "glm/glm.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "glm/gtx/rotate_vector.hpp"

#include "./OpenGlWrapper.h"
#include "./const.h"
#include "./skybox.h"
#include "./utils.h"

#include <cmath>
#include <imgui.h>

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

#include "./Ballon/Ballon.h"
#include "./Environment.h"
#include "./loaderGLTF/loaderGLTF.h"

//https://opengl.developpez.com/tutoriels/apprendre-opengl/?page=systemes-de-coordonnees

int main() {

    auto ctx = p6::Context { { 1280, 720, "Project Ground" } };

    // ctx.maximize_window();

    std::random_device rd;
    std::mt19937 mt(rd());
    std::uniform_real_distribution<double> randPosition(-1.0, 1.0);
    std::uniform_real_distribution<double> randVelocity(-0.005, 0.005);

    Camera camera;
    Boat boat;
    Terrain terrain;
    WaterFrameBuffers waterfbos;
    ShadowFrameBuffers shadowMap;
    Ballon ballon;
    OpenGlWrapper skybox;
    initSkybox(skybox);

    std::vector<Boid> boids;

    for (int i = 0; i < 25; ++i) {
        Boid boid(glm::vec3(randPosition(mt), 1, randPosition(mt)), glm::vec3(randVelocity(mt), randVelocity(mt), randVelocity(mt)));
        boids.emplace_back(boid);
    }

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
        "shaders/shadowGen.vs.glsl",
        "shaders/shadowGen.fs.glsl");

    const p6::Shader shaderSkybox = p6::load_shader(
        "shaders/skybox.vs.glsl",
        "shaders/skybox.fs.glsl");

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
        shaderGLTF.set(uniform_name, value);
    };

    glm::mat4 projection = glm::mat4(1.0f);
    glm::mat4 view = glm::mat4(1.0f);
    glm::mat4 model = glm::mat4(1.0f);

    projection = glm::perspective(glm::radians(45.0f), static_cast<float>(1280) / static_cast<float>(720), 0.001f, 100.0f);
    model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));

    setAllUniform("projection", projection);
    setAllUniform("model", model);
    setAllUniform("normalMatrix", getNormalMatrix(model));

    // glEnable(GL_BLEND);
    // glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    // glBlendColor(1.0f, 1.0f, 1.0f, 1.0f);

    glm::vec3 sunPosition = { -0.5, 0.5, -0.5 };
    glm::vec3 sunColor = { 1.0, 1.0, 1.0 };

    glm::vec3 spotBoatColor = { 1.0, 1.0, 0.90 };
    float spotBoatStrength = 0.8f;
    float spotBoatCutOff = 0.99f;
    float spotBoatOuterCutOff = 0.90f;

    GLuint textureUnit = 0;

    shaderWater.use();
    bindTexture(shaderWater, waterfbos.getReflectionTexture(), "reflectionTexture", textureUnit);
    bindTexture(shaderWater, waterfbos.getRefractionTexture(), "refractionTexture", textureUnit);
    loadAndBindTexture(shaderWater, "assets/textures/waterDUDV.png", "dudvMap", textureUnit);
    loadAndBindTexture(shaderWater, "assets/textures/waterNormalMap.png", "normalMap", textureUnit);

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
    std::vector<std::filesystem::path> faces = { "assets/textures/right.png",
                                                 "assets/textures/left.png",
                                                 "assets/textures/top.png",
                                                 "assets/textures/bottom.png",
                                                 "assets/textures/back.png",
                                                 "assets/textures/front.png" };

    loadAndBindCubemap(shaderSkybox, faces, "skybox", textureUnit);

    glm::mat4 shadowProj = glm::ortho<float>(-2.5, 2.5, -2.5, 2.5, -2.5, 2.5);

    shaderCube.use();

    glActiveTexture(GL_TEXTURE0 + textureUnit + 1); // ?

    loaderGLTF castleCentre("./assets/models/castle/centre/centre.gltf", getModelCasleCentre());
    shaderGLTF.use();
    shaderGLTF.set("projection", projection);

    float coeffSeparation = 1.f;
    float coeffAlignment = 1.f;
    float coeffCohesion = 1.f;
    float distanceGui = 0.4f;
    float maxSpeedBoid = 25.f;

    const char* choiceCamItems[] = { "Default", "Boid", "Top" };
    int choiceCamItemCurrent = 0;
    bool lodBoids = false;

    ctx.imgui = [&]() {
        // Show a simple window
        ImGui::Begin("IHM");

        ImGui::ListBox("Camera", &choiceCamItemCurrent, choiceCamItems, IM_ARRAYSIZE(choiceCamItems));

        if (ImGui::CollapsingHeader("Boids")) {
            ImGui::SliderFloat("Separation", &coeffSeparation, 0.f, 5.f);
            ImGui::SliderFloat("Alignment", &coeffAlignment, 0.f, 5.f);
            ImGui::SliderFloat("Cohesion", &coeffCohesion, 0.f, 5.f);
            ImGui::SliderFloat("Detection range", &distanceGui, 0.1f, 1.f);
            ImGui::SliderFloat("Max speed", &maxSpeedBoid, 0.f, 1000.f);
            ImGui::Checkbox("Low poly boids", &lodBoids);

            if (ImGui::Button("Reset boids")) {
                coeffSeparation = 1.f;
                coeffAlignment = 1.f;
                coeffCohesion = 1.f;
                maxSpeedBoid = 25.f;
            }
        }

        if (ImGui::CollapsingHeader("Sun")) {
            ImGui::SliderFloat3("SunPosition", (float*)&sunPosition, -0.5f, 0.5f);
            ImGui::SliderFloat3("SunColor", (float*)&sunColor, 0.0f, 1.0f);

            if (ImGui::Button("Reset sun")) {
                sunPosition = { -0.5, 0.5, -0.5 };
                sunColor = { 1.0, 1.0, 1.0 };
            }
        }

        if (ImGui::CollapsingHeader("SpotBoat")) {
            ImGui::SliderFloat3("SpotColor", (float*)&spotBoatColor, 0.0f, 1.0f);
            ImGui::SliderFloat("SpotStrength", &spotBoatStrength, 0.f, 1.f);
            ImGui::SliderFloat("SpotCutOff", &spotBoatCutOff, 0.f, 1.f);
            ImGui::SliderFloat("SpotOuterCutOff", &spotBoatOuterCutOff, 0.f, 1.f);

            if (ImGui::Button("Reset spot")) {
                spotBoatColor = { 1.0, 1.0, 0.90 };
                spotBoatStrength = 0.8f;
                spotBoatCutOff = 0.99f;
                spotBoatOuterCutOff = 0.90f;
            }
        }
        ImGui::End();
    };

    // Declare your infinite update loop.
    ctx.update = [&]() {
        //GUI

        setAllUniform("sun.position", sunPosition);
        setAllUniform("sun.color", sunColor);

        setAllUniform("spotBoat.color", spotBoatColor);
        setAllUniform("spotBoat.strength", spotBoatStrength);
        setAllUniform("spotBoat.direction", glm::vec3(0.0, 0.0, 1.0));
        setAllUniform("spotBoat.cutOff", spotBoatCutOff);
        setAllUniform("spotBoat.outerCutOff", spotBoatOuterCutOff);

        p6::ImageSize canvasSize = ctx.main_canvas_size();

        if (choiceCamItemCurrent == 0) { // default
            camera.update(ctx, boat.getPos(), boat.getRot());
        } else if (choiceCamItemCurrent == 1) { // Boid
            camera.update(ctx, boids[0].getPosition(), { 0, 0, 0 });
        } else if (choiceCamItemCurrent == 2) { // Satelite
            camera.update(ctx, { 0, 1.5, 0 }, { 0, 0, 0 });
        }

        const glm::vec3 offsetCenterCamera = glm::vec3(0, 0.04, 0);

        moveWater += WAVE_SPEED * ctx.delta_time();

        boat.update(ctx);

        if (ctx.key_is_pressed(GLFW_KEY_SPACE)) {
            ballon.setActive(boat.getPos());
        }
        ballon.update(ctx);

        setAllUniform("spotBoat.position", boat.getPosLight());
        setAllUniform("spotBoat.direction", boat.getDirection());
        if (ballon.getActive() && choiceCamItemCurrent == 0) {
            camera.update(ctx, ballon.getPosition(), { 0, 0, 0 });
        }

        for (Boid& boid : boids) {
            boid.setMaxSpeed(maxSpeedBoid);
            boid.update(ctx, boids, ballon, coeffSeparation, coeffAlignment, coeffCohesion, distanceGui);
        }

        glm::vec3 posCam = camera.getPos();
        setAllUniform("camPos", posCam);

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
        shaderGLTF.set("projection", shadowProj);
        shaderGLTF.set("view", shadowView);
        boat.draw(shaderGLTF);
        castleCentre.draw(shaderGLTF);
        ballon.draw(shaderGLTF);
        shaderGLTF.set("projection", projection);
        for (Boid& boid : boids) {
            boid.draw(shaderGLTF, lodBoids ? 1 : 0);
        }

        glm::mat4 DepthMVP = shadowProj * shadowView * model;

        setAllUniform("DepthMVP", DepthMVP);
        castleCentre.setDepthMVP(shadowProj, shadowView);
        ballon.setDepthMVP(shadowProj, shadowView);
        boat.setDepthMVP(shadowProj, shadowView);
        for (Boid& boid : boids) {
            boid.setDepthMVP(shadowProj, shadowView);
        }

        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        glViewport(0, 0, canvasSize.width(), canvasSize.height());

        // ---------------------------------
        // RENDER REFLECTION TEXTURE
        // ---------------------------------
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glm::vec3 posCamReflection = posCam;
        float distance = 2 * (posCam.y - OCEAN_HEIGHT);
        posCamReflection.y -= distance;
        glm::vec3 targetCamera = camera.getTargetPos();
        targetCamera.y = -targetCamera.y;
        setAllUniform("view", glm::lookAt(posCamReflection, targetCamera - offsetCenterCamera, { 0, 1, 0 }));
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
        shaderSkybox.set("plane", glm::vec4(0, 0, -1, OCEAN_HEIGHT));
        skybox.draw();

        shaderGLTF.use();
        castleCentre.draw(shaderGLTF);
        ballon.draw(shaderGLTF);
        // boat.draw(shaderGLTF);
        for (Boid& boid : boids) {
            boid.draw(shaderGLTF, lodBoids ? 1 : 0);
        }

        waterfbos.unbindCurrentFrameBuffer();
        glViewport(0, 0, canvasSize.width(), canvasSize.height());

        view = glm::lookAt(posCam, camera.getTargetPos() + offsetCenterCamera, { 0, 1, 0 });
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
        shaderSkybox.set("plane", glm::vec4(0, 0, 1, OCEAN_HEIGHT));
        skybox.draw();

        shaderGLTF.use();
        castleCentre.draw(shaderGLTF);
        ballon.draw(shaderGLTF);
        boat.draw(shaderGLTF);
        for (Boid& boid : boids) {
            boid.draw(shaderGLTF, lodBoids ? 1 : 0);
        }

        waterfbos.unbindCurrentFrameBuffer();
        glViewport(0, 0, canvasSize.width(), canvasSize.height());

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
        shaderSkybox.set("plane", glm::vec4(0, 0, 1, 100));
        skybox.draw();

        shaderWater.use();
        shaderWater.set("moveWater", moveWater);
        terrain.drawWater();

        shaderGLTF.use();
        castleCentre.draw(shaderGLTF);
        ballon.draw(shaderGLTF);
        boat.draw(shaderGLTF);
        for (Boid& boid : boids) {
            boid.draw(shaderGLTF, lodBoids ? 1 : 0);
        }
    };

    // Should be done last. It starts the infinite loop.
    ctx.start();
}