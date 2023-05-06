#include "Ballon.h"
#include "glm/fwd.hpp"

Ballon::Ballon() {
    ballons.emplace_back(loaderGLTF("./assets/models/ballon/ballon.gltf", getModel()));
}

void Ballon::draw(const p6::Shader& shader) {
    if (active) {
        ballons[0].draw(shader);
    }
}

void Ballon::setDepthMVP(const glm::mat4& proj, const glm::mat4& view) {
    ballons[0].setDepthMVP(proj, view);
}

void Ballon::setModelMatrix(const glm::mat4& model) {
    ballons[0].setModelMatrix(model);
}

glm::vec3 Ballon::getPosition() const {
    return position;
}

bool Ballon::getActive() const {
    return active;
}

void Ballon::setActive(glm::vec3 posBoat) {
    if (!active) {
        pv = 10;
        active = true;
        position = posBoat;
        setModel();
    }
}

void Ballon::update(p6::Context& ctx) {
    if (!active) {
        return;
    }

    if (position.y < 1.0) {
        position += glm::vec3(0, 0.05, 0) * ctx.delta_time();
    }
    if (pv <= 0) {
        active = false;
    }

    setModel();
}

void Ballon::setModel() {
    ballons[0].setModelMatrix(getModel());
}

glm::mat4 Ballon::getModel() {
    glm::mat4 model = glm::mat4(1.0);
    model = glm::translate(model, { 0, -0.10, 0 });
    model = glm::translate(model, position);
    model = glm::rotate(model, p6::PI, glm::vec3(1.0f, 0.0f, 0.0f));
    model = glm::scale(model, glm::vec3(0.03));

    return model;
}

void Ballon::hit() {
    pv -= 0.10;
}