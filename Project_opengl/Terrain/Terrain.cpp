#include "Terrain.h"
#include "../utils.h"
#include "glm/fwd.hpp"
#include "glm/trigonometric.hpp"
#include <cmath>

Terrain::Terrain() {
    initTerrain();
    initOcean();
}

void Terrain::initTerrain() {

    std::random_device rd;
    std::mt19937 mt(rd());
    std::uniform_real_distribution<double> offsetZMiniCube(-0.005, 0.005);
    std::uniform_real_distribution<double> offsetXCube(-0.008, 0.008);
    std::uniform_real_distribution<double> offsetYCube(-0.008, 0.008);

    const siv::PerlinNoise::seed_type seed = 33456u;
    const siv::PerlinNoise perlin { seed };

    const siv::PerlinNoise::seed_type seedCube = 35u;
    const siv::PerlinNoise perlinCube { seedCube };

    const siv::PerlinNoise::seed_type seedCubeChoice = 33u;
    const siv::PerlinNoise perlinCubeChoice { seedCubeChoice };

    float di = 0.050; // delta i
    float dj = 0.050; // delta j

    glm::vec3 color_top = { 1.0, 1.0, 1.0 };
    glm::vec3 color_bottom = { 0.2, 0.2, 0.2 };

    for (float i = -1.0; i < 1; i += di) {
        for (float j = -1.0; j < 1; j += dj) {

            float z = getZPerlinTerrain(perlin, i, j);

            if (z > groundLevel) {

                float rand = perlinCube.noise2D(i * 3.0, j * 3.0);

                if (z < 0.20 && rand > -0.2) {
                    //cube & cube below
                    if (perlinCubeChoice.noise2D(i * 3.0, j * 3.0) > 0) {
                        makeCube(i, j, di, dj, z, di, z > groundLevel ? color_top : glm::vec3(0.5, 0.5, 0.5), z > groundLevel ? color_bottom : glm::vec3(0.2, 0.2, 0.2), _openGlWrapperTerrainOr, Cube::BigOr);
                        makeCube(i, j, di, dj, z - di, di, z > groundLevel ? color_top : glm::vec3(0.5, 0.5, 0.5), z > groundLevel ? color_bottom : glm::vec3(0.2, 0.2, 0.2), _openGlWrapperTerrainOr, Cube::BigOr);
                    } else {
                        makeCube(i, j, di, dj, z, di, z > groundLevel ? color_top : glm::vec3(0.5, 0.5, 0.5), z > groundLevel ? color_bottom : glm::vec3(0.2, 0.2, 0.2), _openGlWrapperTerrainAr, Cube::BigAr);
                        makeCube(i, j, di, dj, z - di, di, z > groundLevel ? color_top : glm::vec3(0.5, 0.5, 0.5), z > groundLevel ? color_bottom : glm::vec3(0.2, 0.2, 0.2), _openGlWrapperTerrainAr, Cube::BigAr);
                    }
                } else {
                    float div = 2.0;
                    for (float m = i; m < i + di; m += (di / div)) { // m is the new i
                        for (float n = j; n < j + dj; n += (dj / div)) { // n is the new j & l is the new z
                            float l = getZPerlinTerrain(perlin, m, n);
                            l += offsetZMiniCube(mt);
                            makeCube(m, n, (di / div), (dj / div), l, (di / div), l > groundLevel ? color_top : glm::vec3(0.5, 0.5, 0.5), l > groundLevel ? color_bottom : glm::vec3(0.2, 0.2, 0.2), _openGlWrapperTerrainSmall, Cube::Small);
                            makeCube(m, n, (di / div), (dj / div), l - (di / div), (di / div), l > groundLevel ? color_top : glm::vec3(0.5, 0.5, 0.5), l > groundLevel ? color_bottom : glm::vec3(0.2, 0.2, 0.2), _openGlWrapperTerrainSmall, Cube::Small);
                        }
                    }
                }

            } else {
                makeCube(i, j, di, dj, z, di * 2, z > groundLevel ? color_top : glm::vec3(0.5, 0.5, 0.5), z > groundLevel ? color_bottom : glm::vec3(0.2, 0.2, 0.2), _openGlWrapperTerrainSmall, Cube::Small);
            }
        }
    }

    _openGlWrapperTerrainOr.updateVertices();
    _openGlWrapperTerrainOr.updateTextures();
    _openGlWrapperTerrainOr.updateNormals();

    _openGlWrapperTerrainAr.updateVertices();
    _openGlWrapperTerrainAr.updateTextures();
    _openGlWrapperTerrainAr.updateNormals();

    _openGlWrapperTerrainSmall.updateVertices();
    _openGlWrapperTerrainSmall.updateTextures();
    _openGlWrapperTerrainSmall.updateNormals();
}

void Terrain::initOcean() {

    _openGlWrapperOcean._vertices.push_back(Vertex3D { { -1, -1, 0 }, { 0.5, 0.5, 0.5 } });
    _openGlWrapperOcean._vertices.push_back(Vertex3D { { -1, 1, 0 }, { 0.5, 0.5, 0.5 } });
    _openGlWrapperOcean._vertices.push_back(Vertex3D { { 1, 1, 0 }, { 0.5, 0.5, 0.5 } });
    _openGlWrapperOcean._vertices.push_back(Vertex3D { { 1, -1, 0 }, { 0.5, 0.5, 0.5 } });

    _openGlWrapperOcean._indices.push_back(0);
    _openGlWrapperOcean._indices.push_back(2);
    _openGlWrapperOcean._indices.push_back(1);

    _openGlWrapperOcean._indices.push_back(0);
    _openGlWrapperOcean._indices.push_back(3);
    _openGlWrapperOcean._indices.push_back(2);

    _openGlWrapperOcean.updateVertices();
}

void Terrain::makeCube(float x, float y, float dx, float dy, float z, float height, glm::vec3 colorTop, glm::vec3 colorBottom, OpenGlWrapper& wrapper, Cube type) {

    uint indice = wrapper._indice;

    //top
    wrapper._vertices.push_back(Vertex3D { { x, y, z + height / 2.0 }, colorTop });
    wrapper._vertices.push_back(Vertex3D { { x + dx, y, z + height / 2.0 }, colorTop });
    wrapper._vertices.push_back(Vertex3D { { x + dx, y + dy, z + height / 2.0 }, colorTop });
    wrapper._vertices.push_back(Vertex3D { { x, y + dy, z + height / 2.0 }, colorTop });

    //bottom
    wrapper._vertices.push_back(Vertex3D { { x, y, z - height / 2.0 }, colorBottom });
    wrapper._vertices.push_back(Vertex3D { { x + dx, y, z - height / 2.0 }, colorBottom });
    wrapper._vertices.push_back(Vertex3D { { x + dx, y + dy, z - height / 2.0 }, colorBottom });
    wrapper._vertices.push_back(Vertex3D { { x, y + dy, z - height / 2.0 }, colorBottom });

    wrapper._indices.push_back(indice);
    wrapper._indices.push_back(indice + 4);
    wrapper._indices.push_back(indice + 5);
    wrapper._indices.push_back(indice + 5);
    wrapper._indices.push_back(indice + 1);
    wrapper._indices.push_back(indice + 0);
    repeat(6, [&] {
        wrapper._normals.push_back(glm::vec3(0.0, -1.0, 0.0));
    });

    wrapper._indices.push_back(indice + 3);
    wrapper._indices.push_back(indice + 2);
    wrapper._indices.push_back(indice + 6);
    wrapper._indices.push_back(indice + 6);
    wrapper._indices.push_back(indice + 7);
    wrapper._indices.push_back(indice + 3);
    repeat(6, [&] {
        wrapper._normals.push_back(glm::vec3(0.0, 1.0, 0.0));
    });

    wrapper._indices.push_back(indice);
    wrapper._indices.push_back(indice + 1);
    wrapper._indices.push_back(indice + 2);
    wrapper._indices.push_back(indice + 2);
    wrapper._indices.push_back(indice + 3);
    wrapper._indices.push_back(indice);
    repeat(6, [&] {
        wrapper._normals.push_back(glm::vec3(0.0, 0.0, 1.0));
    });

    wrapper._indices.push_back(indice + 4);
    wrapper._indices.push_back(indice + 7);
    wrapper._indices.push_back(indice + 6);
    wrapper._indices.push_back(indice + 6);
    wrapper._indices.push_back(indice + 5);
    wrapper._indices.push_back(indice + 4);
    repeat(6, [&] {
        wrapper._normals.push_back(glm::vec3(0.0, 0.0, -1.0));
    });

    wrapper._indices.push_back(indice + 1);
    wrapper._indices.push_back(indice + 5);
    wrapper._indices.push_back(indice + 6);
    wrapper._indices.push_back(indice + 6);
    wrapper._indices.push_back(indice + 2);
    wrapper._indices.push_back(indice + 1);
    repeat(6, [&] {
        wrapper._normals.push_back(glm::vec3(1.0, 0.0, 0.0));
    });

    wrapper._indices.push_back(indice + 4);
    wrapper._indices.push_back(indice);
    wrapper._indices.push_back(indice + 3);
    wrapper._indices.push_back(indice + 3);
    wrapper._indices.push_back(indice + 7);
    wrapper._indices.push_back(indice + 4);
    repeat(6, [&] {
        wrapper._normals.push_back(glm::vec3(-1.0, 0.0, 0.0));
    });

    std::random_device rd;
    std::mt19937 mt(rd());
    std::uniform_int_distribution<> distrib(0, 1);

    for (int i = 0; i < 6; ++i) {

        glm::vec2 texId = { 0.0, 0.0 };

        if (type == Cube::BigOr) {
            int id = distrib(mt);
            texId[id] = distrib(mt);

        } else if (type == Cube::BigAr) {
            int id = distrib(mt);
            texId[id] = distrib(mt);
        }

        wrapper._textures.push_back(TextureCube { { 1.0f, 1.0f }, texId });
        wrapper._textures.push_back(TextureCube { { 1.0f, 0.0f }, texId });
        wrapper._textures.push_back(TextureCube { { 0.0f, 0.0f }, texId });
        wrapper._textures.push_back(TextureCube { { 0.0f, 0.0f }, texId });
        wrapper._textures.push_back(TextureCube { { 0.0f, 1.0f }, texId });
        wrapper._textures.push_back(TextureCube { { 1.0f, 1.0f }, texId });
    }

    wrapper._indice += 8;
}

float Terrain::getZPerlinTerrain(siv::PerlinNoise perlin, float x, float y) const {
    return static_cast<float>(perlin.noise2D(x * scaleTerrainPerlin + offsetX, y * scaleTerrainPerlin + offsetY)) * coeffPerlin;
}