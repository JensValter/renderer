#pragma once
#include "input.h"
#include <array>
#include "mat4x4.h"
#include "vec4.h"

class Camera{
    float m_lookSensitivity = 0.01f;
    Vec4 m_up = {0.0f, 1.0f, 0.0f};
    Vec4 m_lookDir= {0,0,1};
    float m_mouseSensitivity = 0.001f;
    float m_lookTheta = 1.57079633f; // pi/2
    float m_lookPhi   = 1.57079633f; // pi/2
    int m_lastX = 0;
    int m_lastY = 0;
    float m_fov = 90.0f;
    float m_near = 0.1f;
    float m_far = 1000.0f;

    public:
    Vec4 camPos = {0.0f, 0.0f, 0.0f};
    void setLookTheta(float theta);
    void setLookPhi(float phi);
    float getLookTheta() const;
    float getLookPhi()const;
    float getLookSensitivity() const;
    Vec4 getUp() const;
    Vec4 getRight() const;
    Vec4 getLookDirection() const;
    Mat4x4 getViewMatrix() const;
    Mat4x4 getProjectionMatrix(int width, int height) const;
    float getMouseSensitivity() const;
    void setMouseSensitivity(float sensitivity);
    
    void processMouseLook(int mouseX, int mouseY);
    void updateCameraDirection();
    

};