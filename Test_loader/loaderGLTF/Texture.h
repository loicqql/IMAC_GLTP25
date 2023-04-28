#pragma once
#include "../utils.h"
#include "p6/p6.h"

// #include <stb/stb_image.h>

// #include "shaderClass.h"

class Texture {
public:
    GLuint ID;
    const char* type;

    Texture(const char* image, const char* texType);

    // Assigns a texture unit to a texture
    void texUnit(GLuint shaderId, const char* uniform, GLuint unit);
    // Binds a texture
    void Bind();
    // Unbinds a texture
    void Unbind();
    // Deletes a texture
    void Delete();
};