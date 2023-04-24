#include "Boid.h"
#include "../utils.h"
#include "glm/fwd.hpp"
#include "glm/geometric.hpp"
#include <cmath>
#include <math.h>

#include "glm/ext/matrix_transform.hpp"
#include "glm/ext/quaternion_transform.hpp"
#include "glm/fwd.hpp"
#include "glm/gtc/type_ptr.hpp"

//Contructors

Boid::Boid() {
    this->_position = glm::vec3(0, 0, 0);
    this->_acceleration = glm::vec3(0, 0, 0);
    this->_velocity = glm::vec3(0, 0, 0);
}

Boid::Boid(glm::vec3 position, glm::vec3 velocity) {
    this->_position = position;
    this->_acceleration = glm::vec3(0, 0, 0);
    this->_velocity = velocity;
}

void Boid::init() {
    // float width = 0.2;

    main.load("./assets/models/boat/boat.gltf");
}

//update && draw

void Boid::update(p6::Context& ctx, std::vector<Boid>& boids) {

    this->_acceleration += this->seek(ctx);

    glm::vec3 separation_force = this->separation(boids);
    separation_force *= 0.08;
    if (!isnan(separation_force.x) && !isnan(separation_force.y)) {
        this->_acceleration += separation_force;
    }

    glm::vec3 alignment_force = this->alignment(boids);
    alignment_force *= 0.08;
    if (!isnan(alignment_force.x) && !isnan(alignment_force.y)) {
        this->_acceleration += alignment_force;
    }

    glm::vec3 cohesion_force = this->cohesion(boids);
    cohesion_force *= 0.00008;
    if (!isnan(cohesion_force.x) && !isnan(cohesion_force.y)) {
        this->_acceleration += cohesion_force;
    }

    this->_velocity += this->_acceleration;
    vec3_limit(this->_velocity, glm::vec3(0.001, 0.001, 0));
    this->_position += this->_velocity;

    this->_acceleration = glm::vec3(0);

    this->_position.x = this->_position.x > 1 ? -1 : this->_position.x;
    this->_position.x = this->_position.x < -1 ? 1 : this->_position.x;
    this->_position.y = this->_position.y > 1 ? -1 : this->_position.y;
    this->_position.y = this->_position.y < -1 ? 1 : this->_position.y;

    ctx.mouse();
}

void Boid::draw(unsigned int modelLoc) {
    glm::mat4 base = glm::mat4(1.0);

    base = glm::translate(base, glm::vec3(_position.x, 0.0, _position.y));
    base = glm::rotate(base, 0.f, glm::vec3(0.0f, 1.0f, 0.0f));
    base = glm::rotate(base, p6::PI / 2.f, glm::vec3(0.0f, 1.0f, 0.0f));
    base = glm::scale(base, glm::vec3(0.015)); // final scale
    base = glm::translate(base, glm::vec3(0.0, 1.0, 0.0)); // fix final z

    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(base));

    main.draw();
}

// rules

glm::vec3 Boid::seek(p6::Context& ctx) {

    glm::vec3 force { 0 };

    float distance = glm::distance(this->_position, glm::vec3(ctx.mouse(), this->_position.z));
    if (distance < 0.3) {
        force = glm::vec3(ctx.mouse(), this->_position.z) - this->_position;
        force = glm::normalize(force);

        force *= 0.005; // max speed

        force -= this->_velocity;

        vec3_limit(force, glm::vec3 { 0.00005 });
    }

    return force;
}

glm::vec3 Boid::separation(std::vector<Boid>& boids) {

    auto totalForce = glm::vec3(0);

    for (Boid& boid : boids) {
        float distance = glm::distance(this->_position, boid._position);
        if (distance > 0 && distance < 0.1) {
            totalForce += (this->_position - boid._position) / distance;
        }
    }

    totalForce = glm::normalize(totalForce);

    totalForce *= 0.05; // max speed

    totalForce -= this->_velocity;

    vec3_limit(totalForce, glm::vec3 { 0.05 });

    return totalForce;
}

glm::vec3 Boid::alignment(std::vector<Boid>& boids) {

    auto totalForce = glm::vec3(0);

    int i = 0;

    for (Boid& boid : boids) {
        float distance = glm::distance(this->_position, boid._position);
        if (distance > 0 && distance < 0.1) {
            ++i;
            totalForce += boid._velocity;
        }
    }

    totalForce /= i;

    totalForce *= 0.05; // max speed

    totalForce -= this->_velocity;

    vec3_limit(totalForce, glm::vec3 { 0.05 });

    return totalForce;
}

glm::vec3 Boid::cohesion(std::vector<Boid>& boids) {

    auto totalForce = glm::vec3(0);

    int i = 0;

    for (Boid& boid : boids) {
        float distance = glm::distance(this->_position, boid._position);
        if (distance > 0 && distance < 0.1) {
            ++i;
            totalForce += boid._position;
        }
    }

    totalForce /= i;

    totalForce *= 0.05; // max speed

    totalForce -= this->_velocity;

    vec3_limit(totalForce, glm::vec3 { 0.05 });

    return totalForce;
}