#pragma once
#include <cstdint>
#include "vec3.h"
#include "mat4x4.h"
#include "vec2.h"
class Triangle
{
public:
    Triangle() = default;
    Triangle(const Vec3& v0, const Vec3& v1, const Vec3& v2);
    Triangle(const Vec3& v0, const Vec3& v1, const Vec3& v2, const Vec2& t0, const Vec2& t1, const Vec2& t2);
    void matrixMultiply(const Mat4x4& mat);
    void applyTransformation(const Mat4x4& mat);
    void add(float x, float y, float z);

    Vec3 normal() const;
    Vec3 toCamera(const Vec3& camera_pos) const;
    
    Vec3 triangle[3];
    Vec2 tex[3];
    uint32_t color;
};

