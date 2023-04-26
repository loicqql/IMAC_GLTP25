#pragma once

#include "../OpenGlWrapper.h"
#include "../PerlinNoise.h"
#include "glm/fwd.hpp"

enum Cube { BigOr,
            BigAr,
            Small };

class Terrain {
private:
    const int width = 60;
    const int height = 60;
    float offsetX = 0.0;
    float offsetY = 0.40;

    float coeffPerlin = 0.5;
    float scaleTerrainPerlin = 2;

    float groundLevel = -0.025;

    OpenGlWrapper _openGlWrapperTerrainOr;
    OpenGlWrapper _openGlWrapperTerrainAr;
    OpenGlWrapper _openGlWrapperTerrainSmall;
    OpenGlWrapper _openGlWrapperOcean;

public:
    Terrain();

    inline void drawTerrain() const {
        drawMountainOr();
        drawMountainAr();
        drawMountainSmall();
    }

    inline void drawMountainOr() const {
        _openGlWrapperTerrainOr.draw();
    }
    inline void drawMountainAr() const {
        _openGlWrapperTerrainAr.draw();
    }
    inline void drawMountainSmall() const {
        _openGlWrapperTerrainSmall.draw();
    }
    inline void drawWater() const {
        _openGlWrapperOcean.draw();
    }

private:
    void initTerrain();

    void initOcean();

    void makeCube(float x, float y, float dx, float dy, float z, float height, glm::vec3 colorTop, glm::vec3 colorBottom, OpenGlWrapper& wrapper, Cube type);

    float getZPerlinTerrain(siv::PerlinNoise perlin, float x, float y) const;
};