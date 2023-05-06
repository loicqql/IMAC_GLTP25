#pragma once
#include "../Ballon/Ballon.h"
#include "../loaderGLTF/loaderGLTF.h"
#include "p6/p6.h"
#include <glm/glm.hpp>
#include <vector>

class Boid {

private:
    glm::vec3 _position {};
    glm::vec3 _acceleration {};
    glm::vec3 _velocity {};

    std::vector<loaderGLTF> _boids; // ?

    float roll = 0.0f;

    const float MAX_X = 1;
    const float MIN_X = -1;
    const float MAX_Z = 1;
    const float MIN_Z = -1;
    const float MAX_Y = 1.5;
    const float MIN_Y = 0.5;

public:
    Boid(glm::vec3 position, glm::vec3 velocity);

    void update(std::vector<Boid>& boids, Ballon& ballon);
    void draw(const p6::Shader& shader);
    void setDepthMVP(const glm::mat4& proj, const glm::mat4& view);

    // Rules
    glm::vec3 seek(Ballon& ballon);
    glm::vec3 separation(std::vector<Boid>& boids);
    glm::vec3 alignment(std::vector<Boid>& boids);
    glm::vec3 cohesion(std::vector<Boid>& boids);
    glm::vec3 returnToCenter();

    inline glm::vec3 getPosition() { return _position; };

private:
    glm::mat4 getModel() const;
    bool outOfBounds() const;
};