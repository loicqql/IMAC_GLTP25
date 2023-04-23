#pragma once

#include "OpenGlWrapper.h"

void initPlane(OpenGlWrapper &plane, glm::vec3 color) {

    plane._vertices.push_back(Vertex3D{{-0.2, 0.2, 0.f}, color});
    plane._vertices.push_back(Vertex3D{{0.2, 0.2, 0.f}, color});
    plane._vertices.push_back(Vertex3D{{0.2, -0.2, 0.f}, color});
    plane._vertices.push_back(Vertex3D{{-0.2, -0.2, 0.f}, color});

    plane._indices.push_back(0);
    plane._indices.push_back(2);
    plane._indices.push_back(1);
    plane._indices.push_back(0);
    plane._indices.push_back(3);
    plane._indices.push_back(2);

    plane.updateVertices();
    plane.updateIndices();
}

void initCube(OpenGlWrapper &cube, glm::vec3 color) {

    uint indice = 0;

    float cube_height2 = 0.02;
    float z = 0.05;

    cube._vertices.push_back(Vertex3D{{-0.02, 0.02, z + cube_height2}, color});
    cube._vertices.push_back(Vertex3D{{0.02, 0.02, z + cube_height2}, color});
    cube._vertices.push_back(Vertex3D{{0.02, -0.02, z + cube_height2}, color});
    cube._vertices.push_back(Vertex3D{{-0.02, -0.02, z + cube_height2}, color});

    //bottom
    cube._vertices.push_back(Vertex3D{{-0.02, 0.02, z - cube_height2}, color});
    cube._vertices.push_back(Vertex3D{{0.02, 0.02, z - cube_height2}, color});
    cube._vertices.push_back(Vertex3D{{0.02, -0.02, z - cube_height2}, color});
    cube._vertices.push_back(Vertex3D{{-0.02, -0.02, z - cube_height2}, color});


    //top
    cube._indices.push_back(indice);
    cube._indices.push_back(indice + 1);
    cube._indices.push_back(indice + 4);
    cube._indices.push_back(indice + 4);
    cube._indices.push_back(indice + 1);
    cube._indices.push_back(indice + 5);

    //Bottom
    cube._indices.push_back(indice + 3);
    cube._indices.push_back(indice + 7);
    cube._indices.push_back(indice + 6);
    cube._indices.push_back(indice + 6);
    cube._indices.push_back(indice + 2);
    cube._indices.push_back(indice + 3);
    
    //front
    cube._indices.push_back(indice);
    cube._indices.push_back(indice + 2);
    cube._indices.push_back(indice + 1);
    cube._indices.push_back(indice);
    cube._indices.push_back(indice + 3);
    cube._indices.push_back(indice + 2);

    //back
    cube._indices.push_back(indice + 4);
    cube._indices.push_back(indice + 5);
    cube._indices.push_back(indice + 6);
    cube._indices.push_back(indice + 4);
    cube._indices.push_back(indice + 6);
    cube._indices.push_back(indice + 7);

    //right
    cube._indices.push_back(indice + 1);
    cube._indices.push_back(indice + 2);
    cube._indices.push_back(indice + 6);
    cube._indices.push_back(indice + 6);
    cube._indices.push_back(indice + 5);
    cube._indices.push_back(indice + 1);

    //left
    cube._indices.push_back(indice + 4);
    cube._indices.push_back(indice + 7);
    cube._indices.push_back(indice + 3);
    cube._indices.push_back(indice + 4);
    cube._indices.push_back(indice + 3);
    cube._indices.push_back(indice + 0);

    cube.updateVertices();
    cube.updateIndices();
}