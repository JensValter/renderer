#include "mat4x4.h"
#include <cmath>


Mat4x4 Mat4x4::identity()
{
    Mat4x4 out = {};

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
    out.matrix[0][2] = -std::sin(theta);

    out.matrix[2][0] = std::sin(theta);
    out.matrix[2][2] = std::cos(theta);

    return out;
}

Mat4x4 Mat4x4::rotationX(float theta)
{
    Mat4x4 out = Mat4x4::identity();

    out.matrix[1][1] = std::cos(theta);
    out.matrix[1][2] = -std::sin(theta);

    out.matrix[2][1] = std::sin(theta);
    out.matrix[2][2] = std::cos(theta);

    return out;
}

Mat4x4 Mat4x4::rotationZ(float theta)
{
    Mat4x4 out = Mat4x4::identity();

    out.matrix[0][0] = std::cos(theta);
    out.matrix[0][1] = -std::sin(theta);

    out.matrix[1][0] = std::sin(theta);
    out.matrix[1][1] = std::cos(theta);

    return out;
}

Mat4x4 Mat4x4::translation(float x, float y, float z)
{
    Mat4x4 out = Mat4x4::identity();

    out.matrix[0][3] = x;
    out.matrix[1][3] = y;
    out.matrix[2][3] = z;

    return out;
}

Mat4x4 Mat4x4::projection(int screen_width, int screen_height, float fov_degrees, float near_plane, float far_plane)
{
    Mat4x4 out = {};

    float fovRad = 1.0f / tanf(fov_degrees * 0.5f / 180.0f * 3.14159f);
    float aspectRatio = float(screen_width) / float(screen_height);

    out.matrix[0][0] = fovRad / aspectRatio;
    out.matrix[1][1] = fovRad;

    out.matrix[2][2] = far_plane / (far_plane - near_plane);
    out.matrix[2][3] = (-far_plane * near_plane) / (far_plane - near_plane);

    out.matrix[3][2] = 1.0f;
    out.matrix[3][3] = 0.0f;

    return out;
}

Mat4x4 Mat4x4::view(Vec3 &camera, Vec3 &target, Vec3 &up)
{
    Vec3 f = (target-camera);
    f.normalizeVector();

    Vec3 r = f.crossProduct(up);
    r.normalizeVector();

    Vec3 u = r.crossProduct(f);

    Mat4x4 out;

    out.matrix[0][0] = r.x;
    out.matrix[0][1] = r.y;
    out.matrix[0][2] = r.z;
    out.matrix[0][3] = -(r.dotProduct(camera));
    out.matrix[1][0] = u.x;
    out.matrix[1][1] = u.y;
    out.matrix[1][2] = u.z;
    out.matrix[1][3] = -(u.dotProduct(camera));
    out.matrix[2][0] = f.x;
    out.matrix[2][1] = f.y;
    out.matrix[2][2] = f.z;
    out.matrix[2][3] = -(f.dotProduct(camera));
    out.matrix[3][0] = 0;
    out.matrix[3][1] = 0;
    out.matrix[3][2] = 0;
    out.matrix[3][3] = 1;

    return out;
}

Vec3 Mat4x4::vecMultiply(const Vec3 &v) const
{
    Vec3 out;

    out.x = matrix[0][0] * v.x + matrix[0][1] * v.y + matrix[0][2] * v.z + matrix[0][3];
    out.y = matrix[1][0] * v.x + matrix[1][1] * v.y + matrix[1][2] * v.z + matrix[1][3];
    out.z = matrix[2][0] * v.x + matrix[2][1] * v.y + matrix[2][2] * v.z + matrix[2][3];

    float w = matrix[3][0] * v.x + matrix[3][1] * v.y + matrix[3][2] * v.z + matrix[3][3];

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
        for (int col = 0; col < 4; col++)
        {
            for (int i = 0; i < 4; i++)
            {
                out.matrix[row][col] += matrix[row][i] * m.matrix[i][col];
            }
        }
    }

    return out;
}