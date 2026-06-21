#pragma once
#include "input.h"
#include <cstdint>

struct Window
{
    void* handle = nullptr;

    int width = 800;
    int height = 600;
    Input input;
    uint32_t* pixels = nullptr;
};

bool CreateAppWindow(Window& window, int width, int height);
void DestroyAppWindow(Window& window);
bool ProcessWindowMessages();
void PresentWindowBuffer(Window& window);