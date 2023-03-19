#pragma once

#include "OpenGlWrapper.h"

#include "PerlinNoise.h"
#include "utils.h"

class Terrain  {
    public:

        int width = 60;
        int height = 60;
        float dx = 0.45;
        float dy = 0.0;
        float coeffPerlin = 0.5;
        float scaleTerrainPerlin = 2;
        float groundLevel = 0.025;

        OpenGlWrapper _openGlWrapper;

        void init() {

            std::random_device rd;
            std::mt19937 mt(rd());
            std::uniform_real_distribution<double> paddingTopBottom(0.0, 0.02);
            std::uniform_real_distribution<double> paddingTopBottomOcean(0.0, 0.008);

            const siv::PerlinNoise::seed_type seed = 123456u;
            const siv::PerlinNoise perlin{ seed };

            uint32_t indice = 0;

            float di = 0.7/(float)width; // delta i
            float dj = 0.7/(float)height; // delta j

            // float voxel_height = 0.03;
            // float voxel_height2 = voxel_height / 2.0;

            glm::vec3 voxel_color_top = {1.0, 1.0, 1.0};
            glm::vec3 voxel_color_bottom = {0.2, 0.2, 0.2};

            glm::vec3 voxel_color_top_ocean = {0.77, 0.67, 0.99};
            glm::vec3 voxel_color_bottom_ocean = {0.47, 0.37, 0.69};

            for(float i = -1.0; i < 1; i += di){
                for(float j = -1.0; j < 1; j += dj){

                    glm::vec3 color_top = voxel_color_top;
                    glm::vec3 color_bottom = voxel_color_bottom;

                    float z = (float)perlin.noise2D(i * scaleTerrainPerlin + dx, j * scaleTerrainPerlin + dy) * coeffPerlin;
                    float zi1 = (float)perlin.noise2D(i * scaleTerrainPerlin + di + dx, j * scaleTerrainPerlin + dy) * coeffPerlin;
                    float zj1 = (float)perlin.noise2D(i * scaleTerrainPerlin + dx, j * scaleTerrainPerlin + dj + dy) * coeffPerlin;

                    z = float_limit(z, groundLevel, true);
                    zi1 = float_limit(zi1, groundLevel, true);
                    zj1 = float_limit(zj1, groundLevel, true);

                    float voxel_height2 = 0.0;

                    if(fabs(z - zi1) < fabs(z - zj1)) { //gen height from derivative 
                        voxel_height2 = fabs(z - zj1) / 2.0;
                    }else {
                        voxel_height2 = fabs(z - zi1) / 2.0;
                    }

                    if(z == groundLevel) {
                        voxel_height2 += paddingTopBottomOcean(mt);

                        color_top = voxel_color_top_ocean;
                        color_bottom = voxel_color_bottom_ocean;

                    }else {
                        voxel_height2 += paddingTopBottom(mt);
                    }

                    //top
                    _openGlWrapper._vertices.push_back(Vertex3D{{i,  j, z + voxel_height2}, color_top});
                    _openGlWrapper._vertices.push_back(Vertex3D{{i + di,  j, z + voxel_height2}, color_top});
                    _openGlWrapper._vertices.push_back(Vertex3D{{i + di,  j + dj, z + voxel_height2}, color_top});
                    _openGlWrapper._vertices.push_back(Vertex3D{{i,  j + dj, z + voxel_height2}, color_top});

                    //bottom
                    _openGlWrapper._vertices.push_back(Vertex3D{{i,  j, z - voxel_height2}, color_bottom});
                    _openGlWrapper._vertices.push_back(Vertex3D{{i + di,  j, z - voxel_height2}, color_bottom});
                    _openGlWrapper._vertices.push_back(Vertex3D{{i + di,  j + dj, z - voxel_height2}, color_bottom});
                    _openGlWrapper._vertices.push_back(Vertex3D{{i,  j + dj, z - voxel_height2}, color_bottom});


                    //top
                    _openGlWrapper._indices.push_back(indice);
                    _openGlWrapper._indices.push_back(indice + 1);
                    _openGlWrapper._indices.push_back(indice + 3);
                    _openGlWrapper._indices.push_back(indice + 1);
                    _openGlWrapper._indices.push_back(indice + 2);
                    _openGlWrapper._indices.push_back(indice + 3);
                    
                    //front
                    _openGlWrapper._indices.push_back(indice + 3);
                    _openGlWrapper._indices.push_back(indice + 2);
                    _openGlWrapper._indices.push_back(indice + 7);
                    _openGlWrapper._indices.push_back(indice + 2);
                    _openGlWrapper._indices.push_back(indice + 6);
                    _openGlWrapper._indices.push_back(indice + 7);

                    //back
                    _openGlWrapper._indices.push_back(indice);
                    _openGlWrapper._indices.push_back(indice + 1);
                    _openGlWrapper._indices.push_back(indice + 4);
                    _openGlWrapper._indices.push_back(indice + 1);
                    _openGlWrapper._indices.push_back(indice + 5);
                    _openGlWrapper._indices.push_back(indice + 4);

                    //right
                    _openGlWrapper._indices.push_back(indice + 1);
                    _openGlWrapper._indices.push_back(indice + 2);
                    _openGlWrapper._indices.push_back(indice + 5);
                    _openGlWrapper._indices.push_back(indice + 2);
                    _openGlWrapper._indices.push_back(indice + 6);
                    _openGlWrapper._indices.push_back(indice + 5);

                    //left
                    _openGlWrapper._indices.push_back(indice);
                    _openGlWrapper._indices.push_back(indice + 3);
                    _openGlWrapper._indices.push_back(indice + 4);
                    _openGlWrapper._indices.push_back(indice + 3);
                    _openGlWrapper._indices.push_back(indice + 7);
                    _openGlWrapper._indices.push_back(indice + 4);

                    indice += 8;

                    _openGlWrapper.updateVertices();
                    _openGlWrapper.updateIndices();
                }
            }
        }

        void draw() const {
            _openGlWrapper.draw();
        }
};