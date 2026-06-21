#pragma once
#include <windows.h>



namespace Key
{
    constexpr int W = 'W';
    constexpr int A = 'A';
    constexpr int S = 'S';
    constexpr int D = 'D';
    constexpr int Space = VK_SPACE;
    constexpr int Ctrl  = VK_CONTROL;
}


struct Input
{
    bool keys[256] = {};
    bool keysPressed[256] = {};
    bool keysReleased[256] = {};


    void OnKeyDown(int vk)
    {
        keysPressed[vk] = true;
        keysReleased[vk] = false;
        keys[vk] = true;
    }

    void OnKeyUp(int vk)
    {   keysReleased[vk] = true;
        keysPressed[vk] = false;
        keys[vk] = false;   
    }

    bool IsDown(int vk) const      { return keys[vk]; }
    bool WasPressed(int vk) const  { return keysPressed[vk]; }
    bool WasReleased(int vk) const { return keysReleased[vk]; }
};