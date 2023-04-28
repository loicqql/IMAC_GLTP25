#include "Texture.h"
#include <iostream>

Texture::Texture(const char* image, const char* texType) {
    type = texType;
    // glActiveTexture(GL_TEXTURE0);
    ID = loadTexture(image);
    // glBindTexture(GL_TEXTURE_2D, ID);
}

void Texture::texUnit(GLuint shaderId, const char* uniform, GLuint unit) {
    // Gets the location of the uniform
    GLuint texUni = glGetUniformLocation(shaderId, uniform);
    // Shader needs to be activated before changing the value of a uniform
    // shader.Activate();
    // Sets the value of the uniform
    glUniform1i(texUni, 0);
}

void Texture::Bind() {
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, ID);
}

void Texture::Unbind() {
    glBindTexture(GL_TEXTURE_2D, 0);
}

void Texture::Delete() {
    glDeleteTextures(1, &ID);
}