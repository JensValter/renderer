#include <MiniFB.h>
#include "window.h"
#include <stdint.h>
#include <string>
#include <iostream>

bool Window::create(int w, int h, const std::string &t)
{
    width = w;
    height = h;
    title = t;
    window = mfb_open_ex(title.c_str(), width, height, MFB_WF_RESIZABLE);
    input.mouseX = mfb_get_mouse_x(window);
    input.mouseY= mfb_get_mouse_y(window);
    isRunning = true;
    buffer = new uint32_t[width * height];
    if (!window)
    {
        return false;
    }
    mfb_set_target_fps(60);
    mfb_set_user_data(window, this);
    mfb_set_mouse_move_callback(window, [](struct mfb_window *window, int x, int y){
        Window* myWindow = (Window*)mfb_get_user_data(window);
        myWindow->input.mouseX = x;
        myWindow->input.mouseY = y;
    });
    mfb_set_keyboard_callback([](struct mfb_window *window, mfb_key key, mfb_key_mod mod, bool is_pressed){
       Window* myWindow = (Window*)mfb_get_user_data(window);
       myWindow->input.keys[(int)key] = is_pressed; }, window);
    timer = mfb_timer_create();
    return true;
}

void Window::close()
{
    if (window != nullptr)
    {
        mfb_close(window);
        window = nullptr;
        isRunning = false;
    }
}

bool Window::draw()
{

    short state = mfb_update_ex(window, buffer, width, height);
    if (state != MFB_STATE_OK)
    {
        this->close();
        return false;
    }

   mfb_wait_sync(window);
    time += mfb_timer_delta(timer);
    ++frames;
    if (time >= 1.0)
    {
        std::string s = title + ", fps: " + std::to_string((frames/time)).substr(0,4);
        mfb_set_title(window, s.c_str());
        frames = 0;
        time = 0;
    }

    return true;
}

RenderBuffer Window::getRenderBuffer() const
{
    return {width, height, buffer};
}
