#include "triangle.h"
#include "mat4x4.h"
#include "rasterMath.h"


Triangle::Triangle(const Vec4& v0, const Vec4& v1, const Vec4& v2)
{
    triangle[0] = v0;
    triangle[1] = v1;
    triangle[2] = v2;

}


void Triangle::vertexSwap(int p1, int p2){
    std::swap(this->triangle[p1], this->triangle[p2]);
    std::swap(this->tex[p1] , this->tex[p2]);
}

Triangle::Triangle(const Vec4& v0, const Vec4& v1, const Vec4& v2, const Vec2& t0, const Vec2& t1, const Vec2& t2){

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
            triangle[i] = mat.vecMultiply(triangle[i]);

            if (triangle[i].w != 0.0f){
                float wInv = 1.0f / triangle[i].w;
                triangle[i].x *= wInv;
                triangle[i].y *= wInv;
                triangle[i].z *= wInv;
               
            }

        }

}

void Triangle::add(float x, float y, float z)
{
    for(Vec4 v: triangle){
        v.x += x;
        v.y += y;
        v.z += z;
    }
}

Vec4 Triangle::normal() const
{
    Vec4 line1 = triangle[1] - triangle[0];
    Vec4 line2 = triangle[2] - triangle[0];

    Vec4 out = line1.crossProduct(line2);

    return out;
}


Vec4 Triangle::toCamera(const Vec4& camera_pos) const
{
    return camera_pos - triangle[0];
}

