#pragma once
#include "renderer3D.h"
#include "mat4x4.h"
#include "object.h"
#include "camera.h"
#include "window.h"
#include <memory>

class Application
{
public:
    bool init(int width, int height, const std::string& windowTitle,const std::string& fileName);
    int run();
    Application() = default;
    

private:
    void update();
    void render();

private:
    Window m_window = {};
    Camera m_camera = {};
    std::unique_ptr<Renderer3D> m_renderer;
    Object m_object;

    float m_theta = 0.0f;
};
