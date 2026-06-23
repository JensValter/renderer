#pragma once
#include "renderer3D.h"
#include "mat4x4.h"
#include "object.h"
#include "window.h"
#include <memory>

class Application
{
public:
    bool init(int width, int height, const std::string& windowTitle,const std::string& fileName);
    int run();
    

private:
    void update();
    void render();
    void drawObject(const Object& object, const Mat4x4& model);

private:
    Window m_window = {};
    std::unique_ptr<Renderer3D> m_renderer;
    Object m_object;

    Mat4x4 m_projection;
    Vec3 m_camera = {0.0f, 0.0f, 0.0f};
    Vec3 m_up = {0.0f, 1.0f, 0.0f};
    Vec3 m_lookDir= {0,0,1};

    float m_theta = 0.0f;
};
