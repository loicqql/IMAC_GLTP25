#include "Boat.h"

#include "glm/ext/matrix_transform.hpp"
#include "glm/ext/quaternion_transform.hpp"
#include "glm/fwd.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "glm/trigonometric.hpp"
#include <cmath>

Boat::Boat() {

    _position = { -.06, 0, -.06 };
    _rotation = { 0, -0.2, 0 };

    objs.emplace_back(loaderGLTF("./assets/models/boat/boat.gltf", getModel()));
}

void Boat::update(p6::Context& ctx) {

    float forceRot = 0.01;
    float limit = 0.0017;

    float coeffRot = 0.01;

    bool leftIsPressed = ctx.key_is_pressed(GLFW_KEY_A);
    bool rightIsPressed = ctx.key_is_pressed(GLFW_KEY_D);
    bool pushIsPressed = ctx.key_is_pressed(GLFW_KEY_W);

    if ((leftIsPressed && rightIsPressed) || pushIsPressed) {
        speed += 0.003 * ctx.delta_time();
        rotRouge += coeffRot;
        rotBleu += coeffRot;
    }
    if (rightIsPressed) {
        _rotation.y += forceRot;
        rotRouge += coeffRot;
        speed -= 0.0003 * ctx.delta_time();
    } else if (leftIsPressed) {
        _rotation.y -= forceRot;
        rotBleu += coeffRot;
        speed -= 0.0003 * ctx.delta_time();
    }

    if (speed > limit) {
        speed = limit;
    }

    if (speed < 0.0) {
        speed = 0.0;
    }

    if (!(leftIsPressed && rightIsPressed)) {
        if (speed > 0.0) {
            speed -= 0.0005 * ctx.delta_time();
            // speed -= 0.00001;
        }
    }

    pitchEffect(ctx);

    glm::vec3 pos = _position;

    _position = { glm::cos(_rotation.y) * (speed * -1.0f) + _position.x, 0, glm::sin(_rotation.y) * (speed * -1.0f) + _position.z };

    if (_position.x > 0.95f || _position.x < -0.95f || _position.z > 0.95f || _position.z < -0.95f) {
        _position = pos;
    }
}

void Boat::draw(const p6::Shader& shader) {
    objs[0].setModelMatrix(getModel());
    objs[0].draw(shader);
}

void Boat::setDepthMVP(const glm::mat4& proj, const glm::mat4& view) {
    objs[0].setDepthMVP(proj, view);
}

void Boat::pitchEffect(p6::Context& ctx) {
    float max = glm::radians(5.f);

    if (pitch) {
        pitchi += 0.5f * ctx.delta_time();
    } else {
        pitchi -= 0.5f * ctx.delta_time();
    }
    if (pitchi > 1) {
        pitch = false;
    }
    if (pitchi < 0) {
        pitch = true;
    }

    _rotation.x = (cubicBezier.valueAt(pitchi, 0) - 0.5) * 2.0 * max;
}

glm::vec3 Boat::getPosLight() {
    float distance = 0.13f;
    return glm::vec3(_position.x - (glm::cos(_rotation.y) * distance), 0.05, _position.z - (glm::sin(_rotation.y) * distance));
}

glm::vec3 Boat::getDirection() {
    return glm::vec3(glm::cos(_rotation.y), glm::sin(_rotation.x), glm::sin(_rotation.y));
}

glm::mat4 Boat::getModel() const {

    glm::mat4 model = glm::mat4(1.0);
    model = glm::translate(model, glm::vec3(_position.x, _position.y, _position.z));
    model = glm::rotate(model, p6::PI, glm::vec3(1.0f, 0.0f, 0.0f));
    model = glm::rotate(model, (-_rotation.x / 2.0f), glm::vec3(glm::sin(_rotation.y) + glm::cos(_rotation.y) / 2, 0.0f, glm::cos(_rotation.y) + glm::sin(_rotation.y) / 2));
    model = glm::rotate(model, _rotation.y, glm::vec3(0.0f, 1.0f, 0.0f));
    model = glm::scale(model, glm::vec3(0.010)); // final scale

    return model;
}