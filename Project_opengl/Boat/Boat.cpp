#include "Boat.h"

#include "glm/ext/matrix_transform.hpp"
#include "glm/ext/quaternion_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

Boat::Boat() {

    _position = { 0.5, 0, 0.1 };
    // _position = {-1, 0, -1};
    _rotation = { 0, 0, 0 };

    main.load("./assets/models/boat/boat.gltf");
    rouge.load("./assets/models/boat/rouge.gltf");
    bleu.load("./assets/models/boat/bleu.gltf");
}

void Boat::update(p6::Context& ctx) {

    float forceRot = 0.01;
    // float limit = 0.0017;
    float limit = 0.17; // dev

    float coeffRot = 0.01;

    bool leftIsPressed = ctx.key_is_pressed(GLFW_KEY_S) || ctx.key_is_pressed(GLFW_KEY_Q);
    bool rightIsPressed = ctx.key_is_pressed(GLFW_KEY_L) || ctx.key_is_pressed(GLFW_KEY_L);

    if (leftIsPressed && rightIsPressed) {
        speed += 0.00005;
        rotRouge += coeffRot;
        rotBleu += coeffRot;
    } else if (leftIsPressed) {
        _rotation.y += forceRot;
        rotRouge += coeffRot;
        speed -= 0.000001;
    } else if (rightIsPressed) {
        _rotation.y -= forceRot;
        rotBleu += coeffRot;
        speed -= 0.000001;
    }

    if (speed > limit) {
        speed = limit;
    }

    if (speed < 0.0) {
        speed = 0.0;
    }

    if (!(leftIsPressed && rightIsPressed)) {
        if (speed > 0.0) {
            speed -= 0.0001;
            // speed -= 0.00001;
        }
    }

    this->_position = { glm::cos(_rotation.y) * (speed * -1.0f) + _position.x, 0, glm::sin(_rotation.y) * (speed * -1.0f) + _position.z };
}

void Boat::draw(unsigned int modelLoc) {

    glm::mat4 base = glm::mat4(1.0);

    base = glm::translate(base, glm::vec3(_position.x, 0.0, _position.z));
    base = glm::rotate(base, -_rotation.y, glm::vec3(0.0f, 1.0f, 0.0f));
    base = glm::rotate(base, p6::PI / 2.f, glm::vec3(0.0f, 1.0f, 0.0f));
    base = glm::scale(base, glm::vec3(0.03)); // final scale
    // base = glm::translate(base, glm::vec3(0.0, 1.0, 0.0)); // fix final z

    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(base));
    main.draw();

    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(glm::rotate(base, -rotBleu, glm::vec3(1.0f, 0.0f, 0.0f))));
    rouge.draw();

    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(glm::rotate(base, -rotRouge, glm::vec3(1.0f, 0.0f, 0.0f))));
    bleu.draw();
}