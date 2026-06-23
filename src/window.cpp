#include <MiniFB.h>
#include "window.h"
#include <stdint.h>
#include <string>
#include <iostream>


bool Window::create(int w, int h, const std::string& t)
{
    width = w;
    height = h;
    title = t;
    isRunning = true;
    buffer = new uint32_t[width * height];
    window = mfb_open_ex(title.c_str(), width, height, MFB_WF_RESIZABLE); 
    if (!window) {
        return false;
    }
    mfb_set_target_fps(60);
    return true;
}

void Window::close(){
    if(window != nullptr) {
        mfb_close(window);
        window = nullptr;
        isRunning = false;
    }
}

bool Window::draw(){
    static auto lastTime = std::chrono::high_resolution_clock::now();
    static int frameCount = 0;

    short state = mfb_update_ex(window, buffer, width, height);
    if (state != MFB_STATE_OK){   
        this->close();
        return false;
    }

    mfb_wait_sync(window);

    frameCount++;
    auto currentTime = std::chrono::high_resolution_clock::now();
    std::chrono::duration<float> elapsed = currentTime - lastTime;

    if (elapsed.count() >= 1.0f) {
        std::string t = title + " fps: " + std::to_string(frameCount);
        mfb_set_title(window,t.c_str());
        frameCount = 0;
        lastTime = currentTime;
    }

    return true;
}

RenderBuffer Window::getRenderBuffer() const
{
     return { width, height, buffer }; 
}
