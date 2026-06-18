#pragma once
#include "vec3.h"

struct Mat4x4;

class Triangle
{
public:
    Triangle() = default;
    Triangle(const Vec3& v0, const Vec3& v1, const Vec3& v2);

    void matrixMultiply(const Mat4x4& mat);
    void toNDC(int width, int height);
    void add(float x, float y, float z);

    Vec3 normal() const;
    Vec3 toCamera(const Vec3& camera_pos) const;

public:
    Vec3 triangle[3];
};