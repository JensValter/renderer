#include "rasterMath.h"

#include <cmath>

uint32_t brightnessModifier(float dotProduct, uint32_t color)
{
    dotProduct = std::abs(dotProduct);

    float r = ((color >> 16) & 0xFF) * dotProduct;
    float g = ((color >> 8) & 0xFF) * dotProduct;
    float b = (color & 0xFF) * dotProduct;

    return ((int)r << 16) | ((int)g << 8) | (int)b;
}

RasterVertex ndcToScreen(const Vec3& ndc, int width, int height)
{
    RasterVertex out;

    out.x = (int)((ndc.x + 1.0f) * width * 0.5f);
    out.y = (int)((1.0f - ndc.y) * height * 0.5f);
    out.z = ndc.z;

    return out;
}