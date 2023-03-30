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
        float dx = 0.45;
        float dy = 0.0;

        float coeffPerlin = 0.5;
        float scaleTerrainPerlin = 2;

        float groundLevel = 0.025;

        float scaleOceanPerlin = 25.0;
        float coeffOcean = 0.01;
        int offsetXOcean = 0;
        int offsetYOcean = 0;

        OpenGlWrapper _openGlWrapperTerrain;
        OpenGlWrapper _openGlWrapperOcean;

    public:

        void initTerrain() {

            std::random_device rd;
            std::mt19937 mt(rd());
            std::uniform_real_distribution<double> paddingTopBottom(0.0, 0.02);
            std::uniform_real_distribution<double> paddingTopBottomOcean(0.0, 0.008);

            const siv::PerlinNoise::seed_type seed = 123456u;
            const siv::PerlinNoise perlin{ seed };

            uint32_t indice = 0;

            float di = 0.7f/static_cast<float>(width); // delta i
            float dj = 0.7f/static_cast<float>(height); // delta j

            glm::vec3 voxel_color_top = {1.0, 1.0, 1.0};
            glm::vec3 voxel_color_bottom = {0.2, 0.2, 0.2};

            for(float i = -1.0; i < 1; i += di){
                for(float j = -1.0; j < 1; j += dj){

                    glm::vec3 color_top = voxel_color_top;
                    glm::vec3 color_bottom = voxel_color_bottom;

                    float z = static_cast<float>(perlin.noise2D(i * scaleTerrainPerlin + dx, j * scaleTerrainPerlin + dy)) * coeffPerlin;
                    float zi1 = static_cast<float>(perlin.noise2D(i * scaleTerrainPerlin + di + dx, j * scaleTerrainPerlin + dy)) * coeffPerlin;
                    float zj1 = static_cast<float>(perlin.noise2D(i * scaleTerrainPerlin + dx, j * scaleTerrainPerlin + dj + dy)) * coeffPerlin;

                    z = float_limit(z, groundLevel, true);
                    zi1 = float_limit(zi1, groundLevel, true);
                    zj1 = float_limit(zj1, groundLevel, true);

                    float voxel_height2 = 0.0;

                    if(std::fabs(z - zi1) < std::fabs(z - zj1)) { //gen height from derivative 
                        voxel_height2 = std::fabs(z - zj1) / 2.0f;
                    }else {
                        voxel_height2 = std::fabs(z - zi1) / 2.0f;
                    }

                    if(z != groundLevel) {

                        voxel_height2 += paddingTopBottom(mt);

                        //top
                        _openGlWrapperTerrain._vertices.push_back(Vertex3D{{i,  j, z + voxel_height2}, color_top});
                        _openGlWrapperTerrain._vertices.push_back(Vertex3D{{i + di,  j, z + voxel_height2}, color_top});
                        _openGlWrapperTerrain._vertices.push_back(Vertex3D{{i + di,  j + dj, z + voxel_height2}, color_top});
                        _openGlWrapperTerrain._vertices.push_back(Vertex3D{{i,  j + dj, z + voxel_height2}, color_top});

                        //bottom
                        _openGlWrapperTerrain._vertices.push_back(Vertex3D{{i,  j, z - voxel_height2}, color_bottom});
                        _openGlWrapperTerrain._vertices.push_back(Vertex3D{{i + di,  j, z - voxel_height2}, color_bottom});
                        _openGlWrapperTerrain._vertices.push_back(Vertex3D{{i + di,  j + dj, z - voxel_height2}, color_bottom});
                        _openGlWrapperTerrain._vertices.push_back(Vertex3D{{i,  j + dj, z - voxel_height2}, color_bottom});


                        //top
                        _openGlWrapperTerrain._indices.push_back(indice);
                        _openGlWrapperTerrain._indices.push_back(indice + 1);
                        _openGlWrapperTerrain._indices.push_back(indice + 3);
                        _openGlWrapperTerrain._indices.push_back(indice + 1);
                        _openGlWrapperTerrain._indices.push_back(indice + 2);
                        _openGlWrapperTerrain._indices.push_back(indice + 3);
                        
                        //front
                        _openGlWrapperTerrain._indices.push_back(indice + 3);
                        _openGlWrapperTerrain._indices.push_back(indice + 2);
                        _openGlWrapperTerrain._indices.push_back(indice + 7);
                        _openGlWrapperTerrain._indices.push_back(indice + 2);
                        _openGlWrapperTerrain._indices.push_back(indice + 6);
                        _openGlWrapperTerrain._indices.push_back(indice + 7);

                        //back
                        _openGlWrapperTerrain._indices.push_back(indice);
                        _openGlWrapperTerrain._indices.push_back(indice + 1);
                        _openGlWrapperTerrain._indices.push_back(indice + 4);
                        _openGlWrapperTerrain._indices.push_back(indice + 1);
                        _openGlWrapperTerrain._indices.push_back(indice + 5);
                        _openGlWrapperTerrain._indices.push_back(indice + 4);

                        //right
                        _openGlWrapperTerrain._indices.push_back(indice + 1);
                        _openGlWrapperTerrain._indices.push_back(indice + 2);
                        _openGlWrapperTerrain._indices.push_back(indice + 5);
                        _openGlWrapperTerrain._indices.push_back(indice + 2);
                        _openGlWrapperTerrain._indices.push_back(indice + 6);
                        _openGlWrapperTerrain._indices.push_back(indice + 5);

                        //left
                        _openGlWrapperTerrain._indices.push_back(indice);
                        _openGlWrapperTerrain._indices.push_back(indice + 3);
                        _openGlWrapperTerrain._indices.push_back(indice + 4);
                        _openGlWrapperTerrain._indices.push_back(indice + 3);
                        _openGlWrapperTerrain._indices.push_back(indice + 7);
                        _openGlWrapperTerrain._indices.push_back(indice + 4);

                        indice += 8;
                    }
                }
            }

            _openGlWrapperTerrain.updateVertices();
            _openGlWrapperTerrain.updateIndices();
        }

        void initOcean() {

            uint32_t indice = 0;

            float di = 1.9f/static_cast<float>(width); // delta i
            float dj = 1.9f/static_cast<float>(height); // delta j

            updateVertices();
            

            for(float i = -1.0; i < 1; i += di){
                for(float j = -1.0; j < 1; j += dj){

                    //top
                    _openGlWrapperOcean._indices.push_back(indice);
                    _openGlWrapperOcean._indices.push_back(indice + 1);
                    _openGlWrapperOcean._indices.push_back(indice + 3);
                    _openGlWrapperOcean._indices.push_back(indice + 1);
                    _openGlWrapperOcean._indices.push_back(indice + 2);
                    _openGlWrapperOcean._indices.push_back(indice + 3);
                    

                    indice += 4;
                }
            }
            _openGlWrapperOcean.updateIndices();
        }

        void update() {
            offsetXOcean -= 1;
            offsetYOcean -= 1;
            updateVertices();
        }

        void draw() const {
            _openGlWrapperTerrain.draw();
            _openGlWrapperOcean.draw();
        }

    private:

        auto getColorAndZ(siv::PerlinNoise &perlin, float i, float j) {

            glm::vec3 top = {0.77, 0.67, 0.99};
            glm::vec3 bottom = {0.47, 0.37, 0.69};

            auto z = static_cast<float>(perlin.noise2D(i * scaleOceanPerlin + offsetXOcean * 0.008, j * scaleOceanPerlin + offsetYOcean * 0.008));

            return std::make_tuple(
                ((z - 0.5f) * coeffOcean) + groundLevel,
                color_map(z, 0, 1, top, bottom)
            );
        }

        void updateVertices() {
            _openGlWrapperOcean._vertices.clear();

            const siv::PerlinNoise::seed_type seed = 123456u;
            siv::PerlinNoise perlin{ seed };

            float di = 1.9f/static_cast<float>(width); // delta i
            float dj = 1.9f/static_cast<float>(height); // delta j


            for(float i = -1.0; i < 1; i += di){
                for(float j = -1.0; j < 1; j += dj) {

                    glm::vec3 color;
                    float z = 0.f;

                    std::tie(z, color) = getColorAndZ(perlin, i, j);
                    _openGlWrapperOcean._vertices.push_back(Vertex3D{{i,  j, z}, color});

                    std::tie(z, color) = getColorAndZ(perlin, i + di, j);
                    _openGlWrapperOcean._vertices.push_back(Vertex3D{{i + di,  j, z}, color});

                    std::tie(z, color) = getColorAndZ(perlin, i + di, j + dj);
                    _openGlWrapperOcean._vertices.push_back(Vertex3D{{i + di,  j + dj, z}, color});

                    std::tie(z, color) = getColorAndZ(perlin, i, j + dj);
                    _openGlWrapperOcean._vertices.push_back(Vertex3D{{i,  j + dj, z}, color});

                }
            }

            _openGlWrapperOcean.updateVertices();
        }

        
};