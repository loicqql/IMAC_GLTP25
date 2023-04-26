#pragma once
#include "./OpenGlWrapper.h"
#include "./Vertex3D.h"
#include "p6/p6.h"
#include <glm/glm.hpp>

inline void initSkybox(OpenGlWrapper& cube) {

    uint indice = 0;

    float cube_height2 = 10.0;
    float z = 0.05;

    //top
    cube._vertices.push_back(Vertex3D { { -10.0, 10.0, z + cube_height2 }, { 0.1, 0, 0 } });
    cube._vertices.push_back(Vertex3D { { 10.0, 10.0, z + cube_height2 }, { 0.1, 0, 0 } });
    cube._vertices.push_back(Vertex3D { { 10.0, -10.0, z + cube_height2 }, { 0.1, 0, 0 } });
    cube._vertices.push_back(Vertex3D { { -10.0, -10.0, z + cube_height2 }, { 0.1, 0, 0 } });

    //bottom
    cube._vertices.push_back(Vertex3D { { -10.0, 10.0, z - cube_height2 }, { 0.2, 0, 0 } });
    cube._vertices.push_back(Vertex3D { { 10.0, 10.0, z - cube_height2 }, { 0.2, 0, 0 } });
    cube._vertices.push_back(Vertex3D { { 10.0, -10.0, z - cube_height2 }, { 0.2, 0, 0 } });
    cube._vertices.push_back(Vertex3D { { -10.0, -10.0, z - cube_height2 }, { 0.2, 0, 0 } });

    //top
    cube._indices.push_back(indice + 5);
    cube._indices.push_back(indice + 1);
    cube._indices.push_back(indice + 4);
    cube._indices.push_back(indice + 4);
    cube._indices.push_back(indice + 1);
    cube._indices.push_back(indice);

    //Bottom
    cube._indices.push_back(indice + 2);
    cube._indices.push_back(indice + 6);
    cube._indices.push_back(indice + 3);
    cube._indices.push_back(indice + 3);
    cube._indices.push_back(indice + 6);
    cube._indices.push_back(indice + 7);

    //front
    cube._indices.push_back(indice + 2);
    cube._indices.push_back(indice + 3);
    cube._indices.push_back(indice);
    cube._indices.push_back(indice);
    cube._indices.push_back(indice + 1);
    cube._indices.push_back(indice + 2);

    //back
    cube._indices.push_back(indice + 7);
    cube._indices.push_back(indice + 6);
    cube._indices.push_back(indice + 5);
    cube._indices.push_back(indice + 5);
    cube._indices.push_back(indice + 4);
    cube._indices.push_back(indice + 7);

    //right
    cube._indices.push_back(indice + 1);
    cube._indices.push_back(indice + 5);
    cube._indices.push_back(indice + 6);
    cube._indices.push_back(indice + 6);
    cube._indices.push_back(indice + 2);
    cube._indices.push_back(indice + 1);

    //left
    cube._indices.push_back(indice + 0);
    cube._indices.push_back(indice + 3);
    cube._indices.push_back(indice + 7);
    cube._indices.push_back(indice + 7);
    cube._indices.push_back(indice + 4);
    cube._indices.push_back(indice + 0);

    cube.updateVertices();
}