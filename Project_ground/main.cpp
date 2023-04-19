#include "glm/ext/matrix_transform.hpp"
#include "glm/ext/vector_float3.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "glm/gtx/rotate_vector.hpp"
#include "glm/glm.hpp"

#include <cmath>

#include "p6/p6.h"

#include "glimac/default_shader.hpp"

#include "Vertex3D.h"
#include "camera.h"
#include "Boat.h"
#include "Terrain.h"

#include "Boid/Boid.h"

#include "Water/WaterFrameBuffers.h"

#include <fstream>

//https://opengl.developpez.com/tutoriels/apprendre-opengl/?page=systemes-de-coordonnees

int main()
{
    auto ctx = p6::Context{{1280, 720, "Project Ground"}};
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


    Camera camera = Camera();
    camera.init();

    Boat boat;
    boat.init();

    Terrain terrain;
    terrain.initTerrain();
    terrain.initOcean();

    WaterFrameBuffers waterfbos;

    GLuint waterDudvTexture = 0;
    loadTexture(waterDudvTexture, "assets/waterDUDV.png");
    GLuint waterNormalTexture = 0;
    loadTexture(waterNormalTexture, "assets/waterNormalMap.png");

    const float WAVE_SPEED = 0.05f;
    float moveWater = 0;

    // glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CLIP_DISTANCE0);

    glEnable(GL_CULL_FACE);
    glCullFace(GL_FRONT);
    glFrontFace(GL_CW);

    glm::mat4 projection = glm::mat4(1.0f);
    projection = glm::perspective(glm::radians(45.0f), static_cast<float>(1280) / static_cast<float>(720), 0.001f, 100.0f);

    const p6::Shader shader = p6::load_shader(
        "shaders/camera.vs.glsl",
        "shaders/camera.fs.glsl"
    );

    const p6::Shader shaderCube = p6::load_shader(
        "shaders/cube.vs.glsl",
        "shaders/cube.fs.glsl"
    );

    const p6::Shader shaderWater = p6::load_shader(
        "shaders/water.vs.glsl",
        "shaders/water.fs.glsl"
    );

    const p6::Shader shaderG = p6::load_shader(
        "shaders/waterG.vs.glsl",
        "shaders/waterG.fs.glsl"
    );

    shader.set("projection", projection);
    shaderCube.set("projection", projection);
    shaderWater.set("projection", projection);
    shaderG.set("projection", projection);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glBlendColor(1.0f, 1.0f, 1.0f, 1.0f);

    waterfbos.check();

    
    glm::vec3 sunColor = {1.0, 1.0, 1.0};
    glm::vec3 sunPosition = {-0.5, 1.0, 0.5};

    // Declare your infinite update loop.
    ctx.update = [&]() {

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        const float OCEAN_HEIGHT = 0.f;

        moveWater += WAVE_SPEED * ctx.delta_time();

        boat.update(ctx);
        camera.update(ctx, boat);
        glm::vec3 posCam = camera.getPos();

        glm::mat4 model = glm::mat4(1.0f);
        glm::mat4 view = glm::mat4(1.0f);
        model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
        view  = glm::translate(view, glm::vec3(0.0f, 0, -2.0f));
        view = glm::lookAt(posCam, boat.getPos(), {0 , 1, 0});

        shaderG.use();
        shaderG.set("model", model);
        shaderG.set("view", view);

        // glm::mat4 projectionOrtho = glm::ortho<float>(-1,1,-1,1,-1,10);
        // shaderG.set("projection", projectionOrtho);

        //invert pitch cam & render reflection texture
        // glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glm::vec3 posCamReflection = posCam;
        float distance = 2 * (posCam.y - OCEAN_HEIGHT);
        posCamReflection.y -= distance;
        shaderG.set("view", glm::lookAt(posCamReflection, boat.getPos(), {0 , 1, 0}));

        shaderG.set("plane", glm::vec4(0, 1, 0, -OCEAN_HEIGHT));
        waterfbos.bindReflectionFrameBuffer();
        // glClearColor(0.2, 0.2, 0.8, 1.0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        terrain.drawMountain();
        waterfbos.unbindCurrentFrameBuffer();
        

        
        shaderG.set("view", view); // reset cam pos


        //render refraction texture
        // glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        shaderG.set("plane", glm::vec4(0, -1, 0, OCEAN_HEIGHT));
        waterfbos.bindRefractionFrameBuffer();
        // glClearColor(0.2, 0.2, 0.8, 1.0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        terrain.drawMountain();

        shaderCube.use();
        shaderCube.set("model", model);
        shaderCube.set("view", view);
        boat.draw(glGetUniformLocation(shaderCube.id(), "model"));
        shaderG.use();
        waterfbos.unbindCurrentFrameBuffer();


        //render to screen
        shader.use();
        // glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        shader.set("model", model);
        shader.set("view", view);
        shader.set("plane", glm::vec4(0, -1, 0, 100));
        shader.set("projection", projection);
        terrain.drawMountain();

        shaderWater.use();
        glActiveTexture(GL_TEXTURE0);
        glUniform1i(glGetUniformLocation(shaderWater.id(), "reflectionTexture"), 0);
        glBindTexture(GL_TEXTURE_2D, waterfbos.getReflectionTexture());
        
        glActiveTexture(GL_TEXTURE1);
        glUniform1i(glGetUniformLocation(shaderWater.id(), "refractionTexture"), 1);
        glBindTexture(GL_TEXTURE_2D, waterfbos.getRefractionTexture());

        glActiveTexture(GL_TEXTURE2);
        glUniform1i(glGetUniformLocation(shaderWater.id(), "dudvMap"), 2);
        glBindTexture(GL_TEXTURE_2D, waterDudvTexture);

        glActiveTexture(GL_TEXTURE3);
        glUniform1i(glGetUniformLocation(shaderWater.id(), "normalMap"), 3);
        glBindTexture(GL_TEXTURE_2D, waterNormalTexture);

        shaderWater.set("lightPosition", sunPosition);
        shaderWater.set("lightColor", sunColor);
        shaderWater.set("moveWater", moveWater);
        shaderWater.set("cameraPosition", posCam);
        shaderWater.set("model", model);
        shaderWater.set("view", view);
        terrain.drawWater();

        shaderCube.use();
        shaderCube.set("model", model);
        shaderCube.set("view", view);

        boat.draw(glGetUniformLocation(shaderCube.id(), "model"));
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