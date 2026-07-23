#include "camera.h"
#include <cmath>
#include "algorithm"

void Camera::setLookTheta(float theta)
{
    m_lookTheta = theta;
}

void Camera::setLookPhi(float phi)
{
    m_lookPhi = phi;
}

float Camera::getLookTheta() const
{
    return m_lookTheta;
}

float Camera::getLookPhi() const
{
    return m_lookPhi;
}

float Camera::getLookSensitivity() const
{
    return m_lookSensitivity;
}

Vec4 Camera::getUp() const
{
    return m_up;
}

Vec4 Camera::getLookDirection() const
{
    return m_lookDir;
}

Mat4x4 Camera::getViewMatrix() const
{
    Vec4 target = camPos + m_lookDir;
    return Mat4x4::view(camPos, target, m_up);
}

Mat4x4 Camera::getProjectionMatrix(int width, int height) const
{
    return Mat4x4::projection(width, height, m_fov, m_near, m_far);
}

float Camera::getMouseSensitivity() const
{
    return m_mouseSensitivity;
}

Vec4 Camera::getRight() const
{
    Vec4 right = m_up.crossProduct(m_lookDir);
    right.normalizeVector();
    return right;
}

void Camera::setMouseSensitivity(float sensitivity)
{
    m_mouseSensitivity = sensitivity;
}

void Camera::processMouseLook(int mouseX, int mouseY)
{
    float deltaX = static_cast<float>(mouseX - m_lastX);
    float deltaY = static_cast<float>(mouseY - m_lastY);

    m_lookTheta += deltaX * m_mouseSensitivity;
    m_lookPhi += deltaY * m_mouseSensitivity;

    updateCameraDirection();

    m_lastX = mouseX;
    m_lastY = mouseY;
}

void Camera::updateCameraDirection()
{
     constexpr float epsilon = 0.01f;
    m_lookPhi = std::clamp(m_lookPhi, epsilon, 3.14159265f - epsilon);

    Vec4 direction;
    direction.z = sinf(m_lookTheta) * sinf(m_lookPhi);
    direction.x = -(cosf(m_lookTheta) * sinf(m_lookPhi));
    direction.y = cosf(m_lookPhi);

    direction.normalizeVector();
    m_lookDir = direction;
}
