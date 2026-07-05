#include "rasterMath.h"
#include "triangle.h"

#include <cmath>

uint32_t brightnessModifier(float dotProduct, uint32_t color)
{
    dotProduct = std::abs(dotProduct);

    float r = ((color >> 16) & 0xFF) * dotProduct;
    float g = ((color >> 8) & 0xFF) * dotProduct;
    float b = (color & 0xFF) * dotProduct;

    return ((int)r << 16) | ((int)g << 8) | (int)b;
}


void ndcToScreen(Triangle& t, int width, int height)
{
    for(int i = 0; i<3; i++){
        t.triangle[i].x = std::round((t.triangle[i].x + 1.0f) * 0.5f * width);
        t.triangle[i].y = std::round((1.0f - t.triangle[i].y) * 0.5f * height);
    }
}
