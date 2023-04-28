#include "Boat.h"

#include "glm/ext/matrix_transform.hpp"
#include "glm/ext/quaternion_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

Boat::Boat() {

    _position = { -0.5, 0, 0.4 };
    // _position = {-1, 0, -1};
    _rotation = { 0, 0, 0 };

    // main.load("./assets/models/boat/boat.gltf");
    // rouge.load("./assets/models/boat/rouge.gltf");
    // bleu.load("./assets/models/boat/bleu.gltf");
}

void Boat::update(p6::Context& ctx) {

    float forceRot = 0.01;
    float limit = 0.0017;

    float coeffRot = 0.01;

    bool leftIsPressed = ctx.key_is_pressed(GLFW_KEY_S) || ctx.key_is_pressed(GLFW_KEY_A);
    bool rightIsPressed = ctx.key_is_pressed(GLFW_KEY_L) || ctx.key_is_pressed(GLFW_KEY_D);

    if (leftIsPressed && rightIsPressed) {
        speed += 0.003 * ctx.delta_time();
        rotRouge += coeffRot;
        rotBleu += coeffRot;
    } else if (leftIsPressed) {
        _rotation.y += forceRot;
        rotRouge += coeffRot;
        speed -= 0.0003 * ctx.delta_time();
    } else if (rightIsPressed) {
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

    this->_position = { glm::cos(_rotation.y) * (speed * -1.0f) + _position.x, 0, glm::sin(_rotation.y) * (speed * -1.0f) + _position.z };
}

void Boat::draw(const p6::Shader& shader) {

    glm::mat4 base = glm::mat4(1.0);

    base = glm::translate(base, glm::vec3(_position.x, 0.0, _position.z));
    base = glm::rotate(base, -_rotation.y, glm::vec3(0.0f, 1.0f, 0.0f));
    base = glm::rotate(base, p6::PI / 2.f, glm::vec3(0.0f, 1.0f, 0.0f));
    base = glm::scale(base, glm::vec3(0.03)); // final scale
    // base = glm::translate(base, glm::vec3(0.0, 1.0, 0.0)); // fix final z

    // shader.set("model", base);
    // main.draw();

    // shader.set("model", base);
    // rouge.draw();

    // shader.set("model", base);
    // bleu.draw();
}