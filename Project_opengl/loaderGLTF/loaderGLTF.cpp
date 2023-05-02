#include "loaderGLTF.h"
#include "../utils.h"
#include "Model.h"

loaderGLTF::loaderGLTF(const char* file, glm::mat4 model) {
    objs.emplace_back(file);
    setModelMatrix(model);
}

void loaderGLTF::draw(const p6::Shader& shader) {
    shader.use();
    shader.set("DepthMVP", DepthMVP);
    shader.set("model", modelMatrix);
    shader.set("normalMatrix", normalMatrix);
    objs[0].Draw(shader.id());
}

void loaderGLTF::setModelMatrix(const glm::mat4& model) {
    modelMatrix = model;
    normalMatrix = getNormalMatrix(model);
}

void loaderGLTF::setDepthMVP(const glm::mat4& proj, const glm::mat4& view) {
    DepthMVP = proj * view * modelMatrix;
}