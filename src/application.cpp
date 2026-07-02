#include "application.h"
#include "rasterMath.h"
#include <iostream>
#include "triangle.h"
#include "mat4x4.h"

bool Application::init(int width, int height, const std::string &windowTitle, const std::string &fileName)
{
    m_window = Window();
    if (!m_window.create(width, height, windowTitle))
    {
        return false;
    }

    m_renderer = std::make_unique<Renderer3D>(m_window.getRenderBuffer());

    if (!m_object.loadObjectFromFile(fileName))
    {
        std::cout << "could not load object\n";
        m_window.close();
        m_renderer.reset();
        return false;
    }

    m_camera = Camera();

    return true;
}

void Application::update()
{
    const float moveSpeed = 0.05f;

    m_camera.processMouseLook(m_window.input.mouseX, m_window.input.mouseY);

    Vec3 forward = m_camera.getLookDirection();
    Vec3 right = m_camera.getRight();

    if (m_window.input.IsDown(Key::W))
        m_camera.camPos += forward * moveSpeed;

    if (m_window.input.IsDown(Key::S))
        m_camera.camPos -= forward * moveSpeed;

    if (m_window.input.IsDown(Key::A))
        m_camera.camPos -= right * moveSpeed;

    if (m_window.input.IsDown(Key::D))
        m_camera.camPos += right * moveSpeed;

    if (m_window.input.IsDown(Key::Space))
        m_camera.camPos.y += moveSpeed;

    if (m_window.input.IsDown(Key::LCtrl))
        m_camera.camPos.y -= moveSpeed;

    if (m_window.input.IsDown(Key::Right))
        m_camera.setLookTheta(m_camera.getLookTheta() + m_camera.getLookSensitivity());

    if (m_window.input.IsDown(Key::Left))
        m_camera.setLookTheta(m_camera.getLookTheta() - m_camera.getLookSensitivity());

    if (m_window.input.IsDown(Key::Up))
        m_camera.setLookPhi(m_camera.getLookPhi() - m_camera.getLookSensitivity());

    if (m_window.input.IsDown(Key::Down))
        m_camera.setLookPhi(m_camera.getLookPhi() + m_camera.getLookSensitivity());

    m_theta += 0.01f;
    if (m_theta >= 2.0f * 3.14159f)
        m_theta = 0.0f;
}

void Application::render()
{
    m_renderer->clear(0x0);
    Mat4x4 model = Mat4x4::translation(0.0f, 0.0f, 10.0f) * Mat4x4::rotationY(m_theta);
    Mat4x4 view = m_camera.getViewMatrix();
    Mat4x4 proj = m_camera.getProjectionMatrix(m_window.width, m_window.height);
    Vec3 light_direction = {0.0f, 0.0f, 1.0f};

    for (const auto& tri : m_object.m_triangles)
    {
        m_renderer->drawTriangle(tri,model , view, proj, m_camera.camPos, light_direction, m_window.width, m_window.height);
    }
    m_window.draw();
}


int Application::run()
{
    while (m_window.isRunning)
    {
        update();
        render();
    }

    m_window.close();
    return 0;
}
