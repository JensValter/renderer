#pragma once
#include <stdint.h>
#include <string>

class Window {
public:
    struct mfb_window* window;
    int width;
    int height;
    bool isRunning = false;
    std::string title;
    uint32_t* buffer;
    ~Window(){
        if(buffer)
        delete(buffer);
    }
    bool create(int w, int h, const std::string& t);
    bool draw();
    void close();
};