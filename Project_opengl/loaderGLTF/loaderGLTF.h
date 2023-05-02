#pragma once
#include "Model.h"
#include "glm/fwd.hpp"
#include "p6/p6.h"

class loaderGLTF {
private:
    glm::mat4 modelMatrix = glm::mat4(1.0);
    glm::mat3 normalMatrix = glm::mat3(1.0);
    glm::mat4 DepthMVP = glm::mat4(1.0);
    std::vector<Model> objs;

public:
    loaderGLTF(const char* file, glm::mat4 model);

    void draw(const p6::Shader& shader);

    void setModelMatrix(const glm::mat4& model);
    void setDepthMVP(const glm::mat4& proj, const glm::mat4& view);

    inline glm::mat4 getModel() { return modelMatrix; };
    inline glm::mat4 getNormal() { return normalMatrix; };
};