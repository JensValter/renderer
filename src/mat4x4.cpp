#include "mat4x4.h"
#include <cmath>

Mat4x4 Mat4x4::identity()
{
    Mat4x4 out;

    out.matrix[0][0] = 1.0f;
    out.matrix[1][1] = 1.0f;
    out.matrix[2][2] = 1.0f;
    out.matrix[3][3] = 1.0f;

    return out;
}

Mat4x4 Mat4x4::rotationY(float theta)
{
    Mat4x4 out = Mat4x4::identity();

    out.matrix[0][0] = std::cos(theta);
    out.matrix[0][2] = std::sin(theta);

    out.matrix[2][0] = -std::sin(theta);
    out.matrix[2][2] = std::cos(theta);

    return out;
}

Mat4x4 Mat4x4::rotationX(float theta)
{
    Mat4x4 out = Mat4x4::identity();

    out.matrix[1][1] = std::cos(theta);
    out.matrix[1][2] = std::sin(theta);

    out.matrix[2][1] = -std::sin(theta);
    out.matrix[2][2] = std::cos(theta);

    return out;
}

Mat4x4 Mat4x4::rotationZ(float theta)
{
    Mat4x4 out = Mat4x4::identity();

    out.matrix[0][0] = std::cos(theta);
    out.matrix[0][1] = std::sin(theta);

    out.matrix[1][0] = -std::sin(theta);
    out.matrix[1][1] = std::cos(theta);

    return out;
}

Mat4x4 Mat4x4::translation(float x, float y, float z)
{
    Mat4x4 out = Mat4x4::identity();

    out.matrix[3][0] = x;
    out.matrix[3][1] = y;
    out.matrix[3][2] = z;

    return out;
}

Mat4x4 Mat4x4::projection(int screen_width, int screen_height, float fov_degrees, float near_plane, float far_plane)
{
    Mat4x4 out;

    float fovRad = 1.0f / tanf(fov_degrees * 0.5f / 180.0f * 3.14159f);
    float aspectRatio = float(screen_width) / float(screen_height);

    out.matrix[0][0] = aspectRatio * fovRad;
    out.matrix[1][1] = fovRad;
    out.matrix[2][2] = far_plane / (far_plane - near_plane);
    out.matrix[3][2] = (-far_plane * near_plane) / (far_plane - near_plane);
    out.matrix[2][3] = 1.0f;
    out.matrix[3][3] = 0.0f;

    return out;
}

Vec3 Mat4x4::vecMultiply(const Vec3 &v) const
{
    Vec3 out;
    out.x = v.x * matrix[0][0] + v.y * matrix[1][0] + v.z * matrix[2][0] + matrix[3][0];
    out.y = v.x * matrix[0][1] + v.y * matrix[1][1] + v.z * matrix[2][1] + matrix[3][1];
    out.z = v.x * matrix[0][2] + v.y * matrix[1][2] + v.z * matrix[2][2] + matrix[3][2];
    float w = v.x * matrix[0][3] + v.y * matrix[1][3] + v.z * matrix[2][3] + matrix[3][3];

    if (w != 0.0f)
    {
        out.x /= w;
        out.y /= w;
        out.z /= w;
    }

    return out;
}

Mat4x4 Mat4x4::operator*(const Mat4x4 &m) const
{
    Mat4x4 out = {};
    for (int row = 0; row < 4; row++)
    {
        for (int i = 0; i < 4; i++)
        {
            float temp = matrix[row][i];
            for (int col = 0; col < 4; col++)
            {
                out.matrix[row][col] += temp * m.matrix[i][col];
            }
        }
    }

    return out;
}
