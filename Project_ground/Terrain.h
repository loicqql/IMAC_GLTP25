#pragma once

#include "OpenGlWrapper.h"

#include "PerlinNoise.h"
#include "glm/fwd.hpp"
#include "glm/trigonometric.hpp"
#include "math.h"
#include "utils.h"
#include <tuple>

class Terrain  {
    private:

        int width = 60;
        int height = 60;
        float dx = 0.0;
        float dy = 0.40;

        float coeffPerlin = 0.5;
        float scaleTerrainPerlin = 2;

        float groundLevel = -0.025;

        
        OpenGlWrapper _openGlWrapperTerrain;
        OpenGlWrapper _openGlWrapperOcean;

    public:

        void initTerrain() {

            std::random_device rd;
            std::mt19937 mt(rd());
            std::uniform_real_distribution<double> paddingTopBottom(0.0, 0.02);
            std::uniform_real_distribution<double> paddingTopBottomOcean(0.0, 0.008);

            const siv::PerlinNoise::seed_type seed = 33456u;
            const siv::PerlinNoise perlin{ seed };

            uint32_t indice = 0;

            float di = 1.5f/static_cast<float>(width); // delta i
            float dj = 1.5f/static_cast<float>(height); // delta j

            glm::vec3 voxel_color_top = {1.0, 1.0, 1.0};
            glm::vec3 voxel_color_bottom = {0.2, 0.2, 0.2};

            for(float i = -1.0; i < 1; i += di){
                for(float j = -1.0; j < 1; j += dj){

                    glm::vec3 color_top = voxel_color_top;
                    glm::vec3 color_bottom = voxel_color_bottom;

                    float z = static_cast<float>(perlin.noise2D(i * scaleTerrainPerlin + dx, j * scaleTerrainPerlin + dy)) * coeffPerlin;
                    float zi1 = static_cast<float>(perlin.noise2D(i * scaleTerrainPerlin + di + dx, j * scaleTerrainPerlin + dy)) * coeffPerlin;
                    float zj1 = static_cast<float>(perlin.noise2D(i * scaleTerrainPerlin + dx, j * scaleTerrainPerlin + dj + dy)) * coeffPerlin;

                    // z = float_limit(z, groundLevel, true);
                    // zi1 = float_limit(zi1, groundLevel, true);
                    // zj1 = float_limit(zj1, groundLevel, true);

                    float voxel_height2 = 0.0;

                    if(std::fabs(z - zi1) < std::fabs(z - zj1)) { //gen height from derivative 
                        voxel_height2 = std::fabs(z - zj1) / 2.0f;
                    }else {
                        voxel_height2 = std::fabs(z - zi1) / 2.0f;
                    }

                    // if(z > groundLevel) {

                        voxel_height2 += paddingTopBottom(mt);

                        //top
                        _openGlWrapperTerrain._vertices.push_back(Vertex3D{{i,  j, z + voxel_height2}, z > groundLevel ? color_top : glm::vec3(0.5, 0.5, 0.5)});
                        _openGlWrapperTerrain._vertices.push_back(Vertex3D{{i + di,  j, z + voxel_height2}, z > groundLevel ? color_top : glm::vec3(0.5, 0.5, 0.5)});
                        _openGlWrapperTerrain._vertices.push_back(Vertex3D{{i + di,  j + dj, z + voxel_height2}, z > groundLevel ? color_top : glm::vec3(0.5, 0.5, 0.5)});
                        _openGlWrapperTerrain._vertices.push_back(Vertex3D{{i,  j + dj, z + voxel_height2}, z > groundLevel ? color_top : glm::vec3(0.5, 0.5, 0.5)});

                        //bottom
                        _openGlWrapperTerrain._vertices.push_back(Vertex3D{{i,  j, z - voxel_height2}, z > groundLevel ? color_bottom : glm::vec3(0.2, 0.2, 0.2)});
                        _openGlWrapperTerrain._vertices.push_back(Vertex3D{{i + di,  j, z - voxel_height2}, z > groundLevel ? color_bottom : glm::vec3(0.2, 0.2, 0.2)});
                        _openGlWrapperTerrain._vertices.push_back(Vertex3D{{i + di,  j + dj, z - voxel_height2}, z > groundLevel ? color_bottom : glm::vec3(0.2, 0.2, 0.2)});
                        _openGlWrapperTerrain._vertices.push_back(Vertex3D{{i,  j + dj, z - voxel_height2}, z > groundLevel ? color_bottom : glm::vec3(0.2, 0.2, 0.2)});


                        //top
                        _openGlWrapperTerrain._indices.push_back(indice);
                        _openGlWrapperTerrain._indices.push_back(indice + 4);
                        _openGlWrapperTerrain._indices.push_back(indice + 1);
                        
                        _openGlWrapperTerrain._indices.push_back(indice + 4);
                        _openGlWrapperTerrain._indices.push_back(indice + 5);
                        _openGlWrapperTerrain._indices.push_back(indice + 1);
                        

                        //Bottom
                        _openGlWrapperTerrain._indices.push_back(indice + 3);
                        _openGlWrapperTerrain._indices.push_back(indice + 6);
                        _openGlWrapperTerrain._indices.push_back(indice + 7);
                        
                        _openGlWrapperTerrain._indices.push_back(indice + 6);
                        _openGlWrapperTerrain._indices.push_back(indice + 3);
                        _openGlWrapperTerrain._indices.push_back(indice + 2);
                        
                        
                        //front
                        _openGlWrapperTerrain._indices.push_back(indice);
                        _openGlWrapperTerrain._indices.push_back(indice + 1);
                        _openGlWrapperTerrain._indices.push_back(indice + 2);
                        
                        _openGlWrapperTerrain._indices.push_back(indice);
                        _openGlWrapperTerrain._indices.push_back(indice + 2);
                        _openGlWrapperTerrain._indices.push_back(indice + 3);
                        
                        //back
                        _openGlWrapperTerrain._indices.push_back(indice + 4);
                        _openGlWrapperTerrain._indices.push_back(indice + 6);
                        _openGlWrapperTerrain._indices.push_back(indice + 5);
                        
                        _openGlWrapperTerrain._indices.push_back(indice + 4);
                        _openGlWrapperTerrain._indices.push_back(indice + 7);
                        _openGlWrapperTerrain._indices.push_back(indice + 6);
                        

                        //right
                        _openGlWrapperTerrain._indices.push_back(indice + 1);
                        _openGlWrapperTerrain._indices.push_back(indice + 6);
                        _openGlWrapperTerrain._indices.push_back(indice + 2);
                        
                        _openGlWrapperTerrain._indices.push_back(indice + 6);
                        _openGlWrapperTerrain._indices.push_back(indice + 1);
                        _openGlWrapperTerrain._indices.push_back(indice + 5);
                        

                        //left
                        _openGlWrapperTerrain._indices.push_back(indice + 4);
                        _openGlWrapperTerrain._indices.push_back(indice + 3);
                        _openGlWrapperTerrain._indices.push_back(indice + 7);
                        
                        _openGlWrapperTerrain._indices.push_back(indice + 4);
                        _openGlWrapperTerrain._indices.push_back(indice + 0);
                        _openGlWrapperTerrain._indices.push_back(indice + 3);
                        

                        indice += 8;
                    // }
                }
            }

            _openGlWrapperTerrain.updateVertices();
            _openGlWrapperTerrain.updateIndices();
        }

        void initOcean() {

            _openGlWrapperOcean._vertices.push_back(Vertex3D{{-1,  -1, 0}, {0.5, 0.5, 0.5}});
            _openGlWrapperOcean._vertices.push_back(Vertex3D{{-1,  1, 0}, {0.5, 0.5, 0.5}});
            _openGlWrapperOcean._vertices.push_back(Vertex3D{{1,  1, 0}, {0.5, 0.5, 0.5}});
            _openGlWrapperOcean._vertices.push_back(Vertex3D{{1,  -1, 0}, {0.5, 0.5, 0.5}});

            _openGlWrapperOcean._indices.push_back(0);
            _openGlWrapperOcean._indices.push_back(2);
            _openGlWrapperOcean._indices.push_back(1);
            
            _openGlWrapperOcean._indices.push_back(0);
            _openGlWrapperOcean._indices.push_back(3);
            _openGlWrapperOcean._indices.push_back(2);
            
            
            _openGlWrapperOcean.updateIndices();
            _openGlWrapperOcean.updateVertices();
        }

        void drawMountain() const {
            _openGlWrapperTerrain.draw();
        }
        void drawWater() const {
            _openGlWrapperOcean.draw();
        }      
};