#include "triangle.h"
#include "mat4x4.h"
#include "rasterMath.h"


Triangle::Triangle(const Vec3& v0, const Vec3& v1, const Vec3& v2)
{
    triangle[0] = v0;
    triangle[1] = v1;
    triangle[2] = v2;

}


void Triangle::vertexSwap(int p1, int p2){
    std::swap(this->triangle[p1], this->triangle[p2]);
    std::swap(this->tex[p1] , this->tex[p2]);
}

Triangle::Triangle(const Vec3& v0, const Vec3& v1, const Vec3& v2, const Vec2& t0, const Vec2& t1, const Vec2& t2){

    triangle[0] = v0;
    triangle[1] = v1;
    triangle[2] = v2;

    tex[0] = t0;
    tex[1] = t1;
    tex[2] = t2;
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

void Triangle::add(float x, float y, float z)
{
    for(Vec3 v: triangle){
        v.x += x;
        v.y += y;
        v.z += z;
    }
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

