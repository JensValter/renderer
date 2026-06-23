#include "application.h"
#include "rasterMath.h"
#include <iostream>
#include "mat4x4.h"

bool Application::init(int width, int height,const std::string& windowTitle, const std::string& fileName)
{
    m_window = Window();
    if (!m_window.create(width, height, windowTitle))
    {
        return false;
    }

    m_renderer = std::make_unique<Renderer3D>(m_window);

    if (!m_object.loadObjectFromFile(fileName))
    {
        std::cout << "could not load object\n";

        m_window.close();
        m_renderer.reset();

        return false;
    }

    return true;
}

int Application::run()
{

    m_projection = Mat4x4::projection(m_window.width, m_window.height, 90.0f, 0.1f, 1000.0f);
    
    while(m_window.isRunning){
    update();
    render();
    }

    m_window.close();

    return 0;
}

void Application::update()
{

    //  const float moveSpeed = 0.01f;

    // if (m_window.input.IsDown(Key::W))
    //     m_camera.z += moveSpeed;

    // if (m_window.input.IsDown(Key::S))
    //     m_camera.z -= moveSpeed;

    // if (m_window.input.IsDown(Key::A))
    //     m_camera.x -= moveSpeed;

    // if (m_window.input.IsDown(Key::D))
    //     m_camera.x += moveSpeed;

    // if (m_window.input.IsDown(Key::Space))
    //     m_camera.y += moveSpeed;

    // if (m_window.input.IsDown(Key::Ctrl))
    //     m_camera.y -= moveSpeed;


   m_theta += 0.001f;
    if (m_theta >= 2.0f * 3.14159f)
    {
        m_theta = 0.0f;
    }
}

void Application::render()
{
    m_renderer->clear(0x0);

    Mat4x4 model =
        Mat4x4::translation(0.0f, 0.0f, 6.0f) *
        Mat4x4::rotationY(m_theta);

    drawObject(m_object, model);

    m_renderer->present();
}

void Application::drawObject(const Object& object, const Mat4x4& model)
{
    Vec3 light_direction = {0.0f, 0.0f, 1.0f};

    Vec3 target = m_camera + m_lookDir;

    Mat4x4 m = m_projection * Mat4x4::view(m_camera,target, m_up);

    for (const auto& tri : object.m_triangles)
    {
        Triangle renderTriangle = tri;

       renderTriangle.matrixMultiply(model);

        Vec3 normal = renderTriangle.normal();
        normal.normalizeVector();

        if (normal.dotProduct(renderTriangle.toCamera(m_camera)) <= 0.0f)
        {
            continue;
        }

        float dotP = normal.dotProduct(light_direction);

        renderTriangle.matrixMultiply(m);

        RasterVertex p0 = ndcToScreen(renderTriangle.triangle[0], m_window.width, m_window.height);
        RasterVertex p1 = ndcToScreen(renderTriangle.triangle[1], m_window.width, m_window.height);
        RasterVertex p2 = ndcToScreen(renderTriangle.triangle[2], m_window.width, m_window.height);

        m_renderer->drawTriangleFill(p0, p1, p2, brightnessModifier(dotP, 0xFFFFFF));

        // m_renderer->drawTriangle(p0, p1, p2, 0xFF0000);
    }
}
