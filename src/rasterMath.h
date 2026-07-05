#pragma once

#include <cstdint>
#include "triangle.h"

uint32_t brightnessModifier(float dotProduct, uint32_t color);

void ndcToScreen(Triangle &t, int width, int height);
