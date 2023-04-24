#pragma once

#include "../loaders/gltf/loaderGLTF.h"
#include "p6/p6.h"
#include <glm/glm.hpp>
#include <vector>

class Boid {

public:
    Boid();
    Boid(glm::vec3 position, glm::vec3 velocity);

private:
    glm::vec3 _position {};
    glm::vec3 _acceleration {};
    glm::vec3 _velocity {};

    loaderGLTF main;

public:
    void init();
    void update(p6::Context& ctx, std::vector<Boid>& boids);
    void draw(unsigned int modelLoc);

    // Rules
    glm::vec3 seek(p6::Context& ctx);
    glm::vec3 separation(std::vector<Boid>& boids);
    glm::vec3 alignment(std::vector<Boid>& boids);
    glm::vec3 cohesion(std::vector<Boid>& boids);
};