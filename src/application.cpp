#include "application.h"
#include "rasterMath.h"
#include <iostream>
#include "mat4x4.h"

bool Application::init(int width, int height, std::string fileName)
{
    if (!CreateAppWindow(m_window, width, height))
    {
        return false;
    }

    m_renderer = std::make_unique<Renderer3D>(m_window);

    m_projection = Mat4x4::projection(m_window.width,m_window.height, 90.0f ,0.1f , 1000.0f);

    if (!m_object.loadObjectFromFile(fileName))
    {
        std::cout << "could not load object\n";

        DestroyAppWindow(m_window);
        m_renderer.reset();

        return false;
    }

    return true;
}

int Application::run()
{
    while (ProcessWindowMessages())
    {
        update();
        render();
    }

    DestroyAppWindow(m_window);

    return 0;
}

void Application::update()
{
    m_theta += 0.001f;

    if (m_theta >= 2.0f * 3.14159f)
    {
        m_theta = 0.0f;
    }
}

void Application::render()
{
    m_renderer->clear(0x0);

    Mat4x4 model = Mat4x4::rotationY(m_theta) * Mat4x4::translation(0.0f,0.0f ,6.0f) ;
    drawObject(m_object, model);

    m_renderer->present();
}



void Application::drawObject(const Object& object, const Mat4x4& model)
{
    Vec3 light_direction = {0.0f, 0.0f, 1.0f};

    for (const auto& tri : object.m_triangles)
    {
        Triangle renderTriangle = tri;

        renderTriangle.matrixMultiply(model);

        //renderTriangle.add(0.0f,0.0f,6.0f);

        Vec3 normal = renderTriangle.normal();
        normal.normalizeVector();

        if (normal.dotProduct(renderTriangle.toCamera(m_camera_pos)) <= 0.0f)
            continue;
    

        float dotP = normal.dotProduct(light_direction);

        renderTriangle.matrixMultiply(m_projection);

        RasterVertex p0 = ndcToScreen(renderTriangle.triangle[0], m_window.width, m_window.height);
        RasterVertex p1 = ndcToScreen(renderTriangle.triangle[1], m_window.width, m_window.height);
        RasterVertex p2 = ndcToScreen(renderTriangle.triangle[2], m_window.width, m_window.height);
      
        m_renderer->drawTriangleFill(p0,p1,p2,brightnessModifier(dotP, 0xFFFFFF));

        // m_renderer->drawTriangle(p0,p1,p2,0xFF0000);
    }
}