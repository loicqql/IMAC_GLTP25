#include "Boid.h"
#include "../utils.h"
#include "glm/fwd.hpp"
#include "glm/geometric.hpp"
#include <cmath>

#include "glm/ext/matrix_transform.hpp"
#include "glm/ext/quaternion_transform.hpp"
#include "glm/fwd.hpp"
#include "glm/gtc/type_ptr.hpp"

#include "glm/gtx/euler_angles.hpp"
#include "glm/gtx/transform.hpp"

//Contructors

Boid::Boid(glm::vec3 position, glm::vec3 velocity) {
    _position = position;
    _acceleration = glm::vec3(0, 0, 0);
    _velocity = velocity;

    _boids.emplace_back(loaderGLTF("./assets/models/plane/plane.gltf", getModel()));
    _boids.emplace_back(loaderGLTF("./assets/models/plane/planeA.gltf", getModel()));

    //helix
    _boids.emplace_back(loaderGLTF("./assets/models/plane/helix.gltf", getModelHelix()));
}

//update && draw

void Boid::update(p6::Context& ctx, std::vector<Boid>& boids, Ballon& ballon, float coeff_separation, float coeff_alignment, float coeff_cohesion, float distance_gui) {

    helix += _velocity.length() * 3.0f * ctx.delta_time();

    float ratio_separation = 0.0008 * coeff_separation;
    float ratio_alignment = 0.0008 * coeff_alignment;
    float ratio_cohesion = 0.0008 * coeff_cohesion;
    float ratio_returnToCenter = (ratio_separation + ratio_alignment + ratio_cohesion) / 3.0f;
    float ratio_seek = 0.008;

    float direction = std::atan2(_velocity.z, _velocity.x);

    if (outOfBounds()) {
        glm::vec3 returnToCenter_force = returnToCenter();
        returnToCenter_force *= ratio_returnToCenter;
        if (_position.y < 0) {
            returnToCenter_force *= 50.f;
        }
        if (vec3_isnan(returnToCenter_force)) {
            _acceleration += returnToCenter_force;
        }
    } else {
        glm::vec3 separation_force = separation(boids, distance_gui);
        separation_force *= ratio_separation;
        if (vec3_isnan(separation_force)) {
            _acceleration += separation_force;
        }

        glm::vec3 alignment_force = alignment(boids, distance_gui);
        alignment_force *= ratio_alignment;
        if (vec3_isnan(alignment_force)) {
            _acceleration += alignment_force;
        }

        glm::vec3 cohesion_force = cohesion(boids, distance_gui);
        cohesion_force *= ratio_cohesion;
        if (vec3_isnan(cohesion_force)) {
            _acceleration += cohesion_force;
        }
    }

    if (ballon.getActive()) {
        glm::vec3 seek_force = seek(ballon);
        seek_force *= ratio_seek;
        if (vec3_isnan(seek_force)) {
            _acceleration += seek_force;
        }
    }

    _velocity += _acceleration;
    vec3_limit(_velocity, glm::vec3(0.01, 0.01, 0.01));
    _position += _velocity;

    // float rollX = _acceleration.x * 500.f;
    // float rollZ = _acceleration.z * 500.f;

    float rotY = (direction - std::atan2(_velocity.z, _velocity.x)) / 3.0f;

    roll += rotY;

    if (std::fabs(roll) > 1.2) { // < 70deg
        roll -= rotY;
    }

    if (rotY < 0.0008) {
        float forceResetRoll = 0.008;

        if (roll >= forceResetRoll) {
            roll -= forceResetRoll;
        } else if (roll <= -forceResetRoll) {
            roll += forceResetRoll;
        }
    }

    //reset
    _acceleration
        = glm::vec3(0);
}

void Boid::draw(const p6::Shader& shader, int idMesh) {
    _boids[idMesh].setModelMatrix(getModel());
    _boids[idMesh].draw(shader);

    _boids[2].setModelMatrix(getModelHelix());
    _boids[2].draw(shader);
}

void Boid::setDepthMVP(const glm::mat4& proj, const glm::mat4& view) {
    _boids[0].setDepthMVP(proj, view);
    _boids[1].setDepthMVP(proj, view);
    _boids[2].setDepthMVP(proj, view);
}

// rules

glm::vec3 Boid::seek(Ballon& ballon) {

    glm::vec3 force { 0 };
    glm::vec3 positionBallon = ballon.getPosition();

    float distance = glm::distance(_position, positionBallon);
    if (distance < 0.18) {
        ballon.hit();
        return glm::vec3(0, 0.2f, 0);
    }
    if (distance < 1.2) {
        force = positionBallon - _position;
        force = glm::normalize(force);

        force *= 0.05; // max speed

        force -= _velocity;

        vec3_limit(force, glm::vec3 { 0.05 });
    }

    return force;
}

glm::vec3 Boid::separation(std::vector<Boid>& boids, float distance_gui) {

    auto totalForce = glm::vec3(0);

    for (Boid& boid : boids) {
        float distance = glm::distance(_position, boid._position);
        if (distance > 0 && distance < distance_gui) {
            totalForce += (_position - boid._position) / distance;
        }
    }

    totalForce = glm::normalize(totalForce);

    totalForce *= 0.05; // max speed

    totalForce -= _velocity;

    vec3_limit(totalForce, glm::vec3 { 0.05 });

    return totalForce;
}

glm::vec3 Boid::alignment(std::vector<Boid>& boids, float distance_gui) {

    auto totalForce = glm::vec3(0);

    int i = 0;

    for (Boid& boid : boids) {
        float distance = glm::distance(_position, boid._position);
        if (distance > 0 && distance < distance_gui) {
            ++i;
            totalForce += boid._velocity;
        }
    }

    totalForce /= i;

    totalForce *= 0.05; // max speed

    totalForce -= _velocity;

    vec3_limit(totalForce, glm::vec3 { 0.05 });

    return totalForce;
}

glm::vec3 Boid::cohesion(std::vector<Boid>& boids, float distance_gui) {

    auto totalForce = glm::vec3(0);

    int i = 0;

    for (Boid& boid : boids) {
        float distance = glm::distance(_position, boid._position);
        if (distance > 0 && distance < distance_gui) {
            ++i;
            totalForce += boid._position;
        }
    }

    totalForce /= i;

    totalForce *= 0.05; // max speed

    totalForce -= _velocity;

    vec3_limit(totalForce, glm::vec3 { 0.05 });

    return totalForce;
}

glm::vec3 Boid::returnToCenter() { // seek with no distance

    glm::vec3 force { 0 };

    force = glm::vec3(0.f, 1.0f, 0.f) - _position;

    force = glm::normalize(force);

    if (_position.y < MIN_Y) {
        force.y *= ((MIN_Y - _position.y) + 1.0f);
    }

    if (_position.y > MAX_Y) {
        force.y *= ((_position.y - MAX_Y) + 1.0f);
    }

    if (_position.y < MIN_Y || _position.y > MAX_Y) {
        force.y *= (fabs(_velocity.y) + 1.0f);
    }

    force *= 0.05; // max speed

    force -= _velocity;

    vec3_limit(force, glm::vec3 { 0.05 });

    return force;
}

glm::mat4 Boid::getModel() const {

    float yaw = std::atan2(_velocity.z, _velocity.x);
    float pitch = _velocity.y * -100.f;

    glm::mat4 model = glm::mat4(1.0);
    glm::mat4 translation = glm::translate(model, glm::vec3(_position.x, _position.y, _position.z));

    glm::mat4 rotation = glm::eulerAngleYXZ(yaw, pitch, roll);

    glm::mat4 rotationFix = glm::rotate(model, p6::PI, glm::vec3(1.0f, 0.0f, 0.0f));

    glm::mat4 scale = glm::scale(model, glm::vec3(0.010)); // final scale

    return translation * rotationFix * rotation * scale;
}

glm::mat4 Boid::getModelHelix() {

    glm::mat4 model = glm::mat4(1.0);
    model = glm::translate(model, glm::vec3(4.7f, -0.9f, 0.0f));
    model = glm::rotate(model, helix, glm::vec3(1.f, 0.f, 0.f));
    model = glm::rotate(model, p6::PI, glm::vec3(0.f, 1.f, 0.f));

    glm::mat4 modelPlane = getModel();

    return modelPlane * model;
}

bool Boid::outOfBounds() const {
    return _position.x > MAX_X || _position.x < MIN_X || _position.z > MAX_Z || _position.z < MIN_Z || _position.y > MAX_Y || _position.y < MIN_Y;
}