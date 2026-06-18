#pragma once

#include "vec3.h"

struct Mat4x4
{
    float matrix[4][4] = {};

    static Mat4x4 identity();
    static Mat4x4 rotationX(float angle);
    static Mat4x4 rotationY(float angle);
    static Mat4x4 rotationZ(float angle);
    static Mat4x4 translation(float x, float y, float z);
    static Mat4x4 projection(int screen_width, int screen_height, float fov_degrees,float near_plane,float far_plane);

    Vec3 vecMultiply(const Vec3& v) const;

    Mat4x4 operator*(const Mat4x4& m) const;
};