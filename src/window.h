#pragma once
#include <string>
#include <stdint.h>
#include "input.h"


struct RenderBuffer {
    int width;
    int height;
    uint32_t* buffer;
};

class Window {
public:
    int width;
    int height;
    std::string title;
    uint32_t* buffer = nullptr;
    struct mfb_window* window = nullptr;
    struct mfb_timer* timer = nullptr;
    int frames = 0;
    double time = 0.0;
    bool isRunning = false;
    bool create(int w, int h, const std::string& t);
    void close();
    bool draw();
    Input input;
    RenderBuffer getRenderBuffer() const;
};