#pragma once
#include <vector>
#include <stdint.h>
#include "rasterMath.h"

struct RenderBuffer;

class Renderer3D
{
private:
    int m_width;
    int m_height;
    uint32_t* m_pixels;
    std::vector<float> m_depthBuffer;

public:
    Renderer3D(RenderBuffer target);

    void clear(uint32_t color);

    void drawPixel(int x, int y, float z, uint32_t color);
    void drawLine(RasterVertex a, RasterVertex b, uint32_t color);
    void drawHorizontalLine(int x0, int x1, int y, float z0, float z1, uint32_t color);

    void drawFlatBottom(RasterVertex v1, RasterVertex v2, RasterVertex v3, uint32_t color);
    void drawFlatTop(RasterVertex v1, RasterVertex v2, RasterVertex v3, uint32_t color);

    void drawTriangle(RasterVertex v1, RasterVertex v2, RasterVertex v3, uint32_t color);
    void drawTriangleFill(RasterVertex v1, RasterVertex v2, RasterVertex v3, uint32_t color);
};