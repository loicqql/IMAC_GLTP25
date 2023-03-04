#include "Boid.h"
#include "glm/fwd.hpp"
#include "glm/geometric.hpp"
#include "utils.h"
#include <cmath>
#include <iostream>
#include <iterator>

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

    // this->acceleration += this->seek(ctx);

    glm::vec2 separation_force = this->separation(boids);

    glm::normalize(separation_force);

    separation_force *= 0.00005;

    this->acceleration += separation_force;

    this->velocity += this->acceleration;
    // vec2_limit(this->velocity, glm::vec2(0.001, 0.001));
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
    glm::vec2 force = ctx.mouse() - this->position;
    force = glm::normalize(force);

    force *= 0.005; // max speed

    force -= this->velocity;

    vec2_limit(force, glm::vec2{0.00005});

    return force;
}


glm::vec2 Boid::separation(std::vector<Boid> &boids) {

    glm::vec2 totalForce = glm::vec2(0);

    for (Boid &boid : boids) {
        float distance = glm::distance(this->position, boid.position);
        if(distance > 0 && distance < 0.1) {
            totalForce += (this->position - boid.position) / distance;
        }
    }

    return totalForce;
}