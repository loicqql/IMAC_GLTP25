#pragma once

#include "../bezier.h"
#include "glm/fwd.hpp"
#include "p6/p6.h"

class Boat {
private:
    glm::vec3 _position {};
    glm::vec3 _rotation {};

    // loaderGLTF main;
    // loaderGLTF rouge;
    // loaderGLTF bleu;

    float width = 0.02;
    float speed = 0.0;
    float rotBleu = 0.0;
    float rotRouge = 0.0;

    bool pitch = false;
    Bezier::Bezier<3> cubicBezier = Bezier::Bezier<3>({ { 0, 1 }, { 0.1, 1 }, { 0.9, 0 }, { 1, 0 } });
    float pitchi = 0;

public:
    Boat();

    void update(p6::Context& ctx);

    void draw(const p6::Shader& shader);

    inline glm::vec3 getPos() { return _position; }

    glm::vec3 getPosLight();
    glm::vec3 getDirection();

    inline glm::vec3 getRot() { return _rotation; }

private:
    void pitchEffect(p6::Context& ctx);
};