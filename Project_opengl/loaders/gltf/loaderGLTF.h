#pragma once

#include <iostream>

#include "p6/p6.h"

#include "./tiny_gltf.h"

#define BUFFER_OFFSET(i) ((char*)NULL + (i))

// https://github.com/syoyo/tinygltf/blob/release/examples/basic/main.cpp

class loaderGLTF {

private:
    tinygltf::Model model;
    std::pair<GLuint, std::map<int, GLuint>> vaoAndEbos;

public:
    void load(const char* filename);

    void draw();

    // ~loaderGLTF();

private:
    bool loadModel(tinygltf::Model& model, const char* filename);

    void bindMesh(std::map<int, GLuint>& vbos, tinygltf::Model& model, tinygltf::Mesh& mesh);

    // bind models
    void bindModelNodes(std::map<int, GLuint>& vbos, tinygltf::Model& model, tinygltf::Node& node);

    std::pair<GLuint, std::map<int, GLuint>> bindModel(tinygltf::Model& model);

    void drawMesh(const std::map<int, GLuint>& vbos, tinygltf::Model& model, tinygltf::Mesh& mesh);

    // recursively draw node and children nodes of model
    void drawModelNodes(const std::pair<GLuint, std::map<int, GLuint>>& vaoAndEbos, tinygltf::Model& model, tinygltf::Node& node);

    void drawModel(const std::pair<GLuint, std::map<int, GLuint>>& vaoAndEbos, tinygltf::Model& model);

    void dbgModel(tinygltf::Model& model);
};