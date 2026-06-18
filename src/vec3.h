#pragma once

struct Vec3
{
    float x = 0.0f;
    float y = 0.0f;
    float z = 0.0f;

    Vec3 operator+(const Vec3& v) const;
    Vec3 operator-(const Vec3& v) const;
    Vec3 operator*(float value) const;

    float dotProduct(const Vec3& v) const;
    Vec3 crossProduct(const Vec3& v) const;

    void normalizeVector();
    
};