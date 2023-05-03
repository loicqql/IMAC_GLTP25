#pragma once
#include "../loaderGLTF/loaderGLTF.h"
#include "glm/fwd.hpp"
#include "p6/p6.h"
#include <vector>

class Ballon {
private:
    std::vector<loaderGLTF> ballons;
    bool active = false;
    glm::vec3 position = { -0.5, 0, 0 };
    int pv = 0;

public:
    Ballon();

    void draw(const p6::Shader& shader);
    void setDepthMVP(const glm::mat4& proj, const glm::mat4& view);
    void setModelMatrix(const glm::mat4& model);
    glm::vec3 getPosition() const;
    bool getActive() const;
    void setActive(glm::vec3 posBoat);

    void update(p6::Context& ctx);

private:
    void setModel();
    glm::mat4 getModel();
};