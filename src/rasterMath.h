#pragma once

#include <cstdint>

#include "vec3.h"

struct RasterVertex
{
    int x = 0;
    int y = 0;
    float z = 0.0f;

};

struct Vec2{
    float u = 0.0f;
    float v = 0.0f;
};

uint32_t brightnessModifier(float dotProduct, uint32_t color);

RasterVertex ndcToScreen(const Vec3& ndc, int width, int height);
