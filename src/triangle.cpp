#include "triangle.h"
#include "mat4x4.h"
#include "rasterMath.h"

Triangle::Triangle(const Vec3& v0, const Vec3& v1, const Vec3& v2)
{
    triangle[0] = v0;
    triangle[1] = v1;
    triangle[2] = v2;
}

void Triangle::matrixMultiply(const Mat4x4& mat)
{
        for(int i = 0; i<3; i++)
            triangle[i] = mat.vecMultiply(triangle[i]);
}

void Triangle::applyTransformation(const Mat4x4& mat)
{
        for(int i = 0; i<3; i++){
            float w = mat.getW(triangle[i]);
            triangle[i] = mat.vecMultiply(triangle[i]);

            if (w != 0.0f)
            triangle[i] *= (1.0f / w); 
        }

}

void Triangle::toNDC(int width, int height)
{
    ndcToScreen(triangle[0],width, height);
    ndcToScreen(triangle[1],width, height);
    ndcToScreen(triangle[2],width, height);
}

void Triangle::add(float x, float y, float z)
{
    triangle[0].x += x;
    triangle[0].y += y;
    triangle[0].z += z;

    triangle[1].x += x;
    triangle[1].y += y;
    triangle[1].z += z;

    triangle[2].x += x;
    triangle[2].y += y;
    triangle[2].z += z;
}



Vec3 Triangle::normal() const
{
    Vec3 line1 = triangle[1] - triangle[0];
    Vec3 line2 = triangle[2] - triangle[0];

    Vec3 out = line1.crossProduct(line2);

    return out;
}

Vec3 Triangle::toCamera(const Vec3& camera_pos) const
{
    return camera_pos - triangle[0];
}