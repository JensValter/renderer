#pragma once

struct Vec3
{
    float x = 0.0f;
    float y = 0.0f;
    float z = 0.0f;

    Vec3 operator+(const Vec3& v) const;
    Vec3 operator-(const Vec3& v) const;
    Vec3 operator*(float value) const;
    Vec3 operator *= (float value);
    Vec3& operator+=(const Vec3& v);
    Vec3& operator-=(const Vec3& v);

    float dotProduct(const Vec3& v) const;
    Vec3 crossProduct(const Vec3& v) const;

    void normalizeVector();
    
};