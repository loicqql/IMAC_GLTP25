#include "Boid.h"
#include "glm/fwd.hpp"
#include "glm/geometric.hpp"
#include "utils.h"
#include <cmath>
#include <math.h>

//Contructors

Boid::Boid() {
    this->position = glm::vec2(0, 0);
    this->acceleration = glm::vec2(0, 0);
    this->velocity = glm::vec2(0, 0);
}

Boid::Boid(glm::vec2 position, glm::vec2 velocity) {
    this->position = position;
    this->acceleration = glm::vec2(0, 0);
    this->velocity = velocity;
}

//update && draw

void Boid::update(p6::Context &ctx, std::vector<Boid> &boids) {

    this->acceleration += this->seek(ctx);

    glm::vec2 separation_force = this->separation(boids);
    separation_force *= 0.08;
    if(!isnan(separation_force.x) && !isnan(separation_force.y)) {
        this->acceleration += separation_force;
    }

    glm::vec2 alignment_force = this->alignment(boids);
    alignment_force *= 0.08;
    if(!isnan(alignment_force.x) && !isnan(alignment_force.y)) {
        this->acceleration += alignment_force;
    }

    glm::vec2 cohesion_force = this->cohesion(boids);
    cohesion_force *= 0.00008;
    if(!isnan(cohesion_force.x) && !isnan(cohesion_force.y)) {
        this->acceleration += cohesion_force;
    }

    this->velocity += this->acceleration;
    vec2_limit(this->velocity, glm::vec2(0.001, 0.001));
    this->position += this->velocity;

    this->acceleration = glm::vec2(0);

    this->position.x = this->position.x > 1 ? -1 : this->position.x;
    this->position.x = this->position.x < -1 ? 1 : this->position.x;
    this->position.y = this->position.y > 1 ? -1 : this->position.y;
    this->position.y = this->position.y < -1 ? 1 : this->position.y;

    
}

void Boid::draw(p6::Context &ctx) {
    ctx.equilateral_triangle(p6::Center{this->position}, p6::Radius(0.05), p6::Rotation(p6::Radians(vec2_get_direction(this->velocity))));
}


// rules

glm::vec2 Boid::seek(p6::Context &ctx) {

    glm::vec2 force{0};

    float distance = glm::distance(this->position, ctx.mouse());
    if(distance < 0.3) {
        force = ctx.mouse() - this->position;
        force = glm::normalize(force);

        force *= 0.005; // max speed

        force -= this->velocity;

        vec2_limit(force, glm::vec2{0.00005});
    }    

    return force;
}


glm::vec2 Boid::separation(std::vector<Boid> &boids) {

    auto totalForce = glm::vec2(0);

    for (Boid &boid : boids) {
        float distance = glm::distance(this->position, boid.position);
        if(distance > 0 && distance < 0.1) {
            totalForce += (this->position - boid.position) / distance;
        }
    }

    totalForce = glm::normalize(totalForce);

    totalForce *= 0.05; // max speed

    totalForce -= this->velocity;

    vec2_limit(totalForce, glm::vec2{0.05});

    return totalForce;
}

glm::vec2 Boid::alignment(std::vector<Boid> &boids) {

    auto totalForce = glm::vec2(0);

    int i = 0;

    for (Boid &boid : boids) {
        float distance = glm::distance(this->position, boid.position);
        if(distance > 0 && distance < 0.1) {
            ++i;
            totalForce += boid.velocity;
        }
    }

    totalForce /= i;

    totalForce *= 0.05; // max speed

    totalForce -= this->velocity;

    vec2_limit(totalForce, glm::vec2{0.05});

    return totalForce;
}

glm::vec2 Boid::cohesion(std::vector<Boid> &boids) {

    auto totalForce = glm::vec2(0);

    int i = 0;

    for (Boid &boid : boids) {
        float distance = glm::distance(this->position, boid.position);
        if(distance > 0 && distance < 0.1) {
            ++i;
            totalForce += boid.position;
        }
    }

    totalForce /= i;

    totalForce *= 0.05; // max speed

    totalForce -= this->velocity;

    vec2_limit(totalForce, glm::vec2{0.05});

    return totalForce;
}