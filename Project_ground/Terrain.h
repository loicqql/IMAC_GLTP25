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

        const int width = 60;
        const int height = 60;
        float offsetX = 0.0;
        float offsetY = 0.40;

        float coeffPerlin = 0.5;
        float scaleTerrainPerlin = 2;

        float groundLevel = -0.025;

        
        OpenGlWrapper _openGlWrapperTerrain;
        OpenGlWrapper _openGlWrapperOcean;

    public:

        void initTerrain() {

            std::random_device rd;
            std::mt19937 mt(rd());
            std::uniform_real_distribution<double> offsetZMiniCube(-0.005, 0.005);
            std::uniform_real_distribution<double> offsetXCube(-0.008, 0.008);
            std::uniform_real_distribution<double> offsetYCube(-0.008, 0.008);

            const siv::PerlinNoise::seed_type seed = 33456u;
            const siv::PerlinNoise perlin{ seed };

            const siv::PerlinNoise::seed_type seedCube = 35u;
            const siv::PerlinNoise perlinCube{ seedCube };

            uint32_t indice = 0;

            float di = 0.050; // delta i
            float dj = 0.050; // delta j

            glm::vec3 color_top = {1.0, 1.0, 1.0};
            glm::vec3 color_bottom = {0.2, 0.2, 0.2};

            for(float i = -1.0; i < 1; i += di){
                for(float j = -1.0; j < 1; j += dj){

                    float z = getZPerlinTerrain(perlin, i, j);

                    if(z > groundLevel) {

                        float rand = perlinCube.noise2D(i * 3.0, j * 3.0);

                        if(z < 0.20 && rand > -0.2) {
                            //cube & cube below
                            makeCube(i, j, di, dj, z, di, z > groundLevel ? color_top : glm::vec3(0.5, 0.5, 0.5), z > groundLevel ? color_bottom : glm::vec3(0.2, 0.2, 0.2), indice);
                            makeCube(i, j, di, dj, z - di, di, z > groundLevel ? color_top : glm::vec3(0.5, 0.5, 0.5), z > groundLevel ? color_bottom : glm::vec3(0.2, 0.2, 0.2), indice);
                        }else {
                            float div = 3.0;
                            for(float m = i; m < i + di; m += (di / div)) { // m is the new i
                                for(float n = j; n < j + dj; n += (dj / div)) { // n is the new j & l is the new z
                                    float l = getZPerlinTerrain(perlin, m, n);
                                    l += offsetZMiniCube(mt);
                                    makeCube(m, n, (di / div), (dj / div), l, (di / div), l > groundLevel ? color_top : glm::vec3(0.5, 0.5, 0.5), l > groundLevel ? color_bottom : glm::vec3(0.2, 0.2, 0.2), indice);
                                    makeCube(m, n, (di / div), (dj / div), l - (di / div), (di / div), l > groundLevel ? color_top : glm::vec3(0.5, 0.5, 0.5), l > groundLevel ? color_bottom : glm::vec3(0.2, 0.2, 0.2), indice);
                                }
                            }
                        }
                        
                    }else {
                        makeCube(i, j, di, dj, z, di * 2, z > groundLevel ? color_top : glm::vec3(0.5, 0.5, 0.5), z > groundLevel ? color_bottom : glm::vec3(0.2, 0.2, 0.2), indice);
                    }
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

    private : 

        void makeCube(float x, float y, float dx, float dy, float z, float height, glm::vec3 colorTop, glm::vec3 colorBottom, uint &indice) {

            //top
            _openGlWrapperTerrain._vertices.push_back(Vertex3D{{x,  y, z + height / 2.0}, colorTop});
            _openGlWrapperTerrain._vertices.push_back(Vertex3D{{x + dx,  y, z + height / 2.0}, colorTop});
            _openGlWrapperTerrain._vertices.push_back(Vertex3D{{x + dx,  y + dy, z + height / 2.0}, colorTop});
            _openGlWrapperTerrain._vertices.push_back(Vertex3D{{x,  y + dy, z + height / 2.0}, colorTop});

            //bottom
            _openGlWrapperTerrain._vertices.push_back(Vertex3D{{x,  y, z - height / 2.0}, colorBottom});
            _openGlWrapperTerrain._vertices.push_back(Vertex3D{{x + dx,  y, z - height / 2.0}, colorBottom});
            _openGlWrapperTerrain._vertices.push_back(Vertex3D{{x + dx,  y + dy, z - height / 2.0}, colorBottom});
            _openGlWrapperTerrain._vertices.push_back(Vertex3D{{x,  y + dy, z - height / 2.0}, colorBottom});


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
        }

        float getZPerlinTerrain(siv::PerlinNoise perlin, float x, float y) const {
            return static_cast<float>(perlin.noise2D(x * scaleTerrainPerlin + offsetX, y * scaleTerrainPerlin + offsetY)) * coeffPerlin;
        }
};