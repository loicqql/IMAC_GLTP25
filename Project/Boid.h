#pragma once

#include <glm/glm.hpp>
#include <vector>
#include "p6/p6.h"

class Boid {

    public : 
        Boid();
        Boid(glm::vec2 position, glm::vec2 velocity);

        glm::vec2 position{};
        glm::vec2 acceleration{};
        glm::vec2 velocity{};


    public : 
        void update(p6::Context &ctx, std::vector<Boid> &boids);
        void draw(p6::Context &ctx);

        // Rules
        glm::vec2 seek(p6::Context &ctx);
        glm::vec2 separation(std::vector<Boid> &boids);
        glm::vec2 alignment(std::vector<Boid> &boids);
        glm::vec2 cohesion(std::vector<Boid> &boids);

};