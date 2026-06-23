#pragma once
#include <string>
#include <stdint.h>

struct mfb_window;

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
    bool isRunning = false;
    bool create(int w, int h, const std::string& t);
    void close();
    bool draw();
    RenderBuffer getRenderBuffer() const;
};