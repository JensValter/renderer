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

int Triangle::planeClipping(const Vec3 &p0, const Vec3 &n, Triangle &in_t, Triangle &out_t1, Triangle &out_t2)
{
    Vec3 normal = n;
    normal.normalizeVector();

    auto distance = [&](const Vec3 &p)
    {
        return normal.dotProduct(p - p0);
    };

    Vec3* inside_points[3]; 
    int insidePointCount = 0;

    Vec3* outside_points[3];
    int outsidePointsCount = 0;

    for(int i = 0; i<3; i++){
        float dist = distance(in_t.triangle[i]);
        dist >= 0 ? inside_points[insidePointCount++] = &in_t.triangle[i] : outside_points[outsidePointsCount++] = &in_t.triangle[i];
    }

    if(insidePointCount == 0){
        return 0;
    }

    else if(insidePointCount == 3){
        out_t1 = in_t;
        return 1;
    }

    else if(insidePointCount == 1){
        out_t1.triangle[0] = *inside_points[0];

        out_t1.triangle[1] = Vec3::vecPlanIntersect(p0, normal, *inside_points[0], *outside_points[0]);

        out_t1.triangle[2] = Vec3::vecPlanIntersect(p0, normal, *inside_points[0], *outside_points[1]);

        return 1;
    }

    else if(insidePointCount == 2){
        Vec3 intersect1 = Vec3::vecPlanIntersect(p0, normal, *inside_points[0], *outside_points[0]);

        Vec3 intersect2 = Vec3::vecPlanIntersect(p0, normal, *inside_points[1], *outside_points[0]);

        out_t1.triangle[0] = *inside_points[0];
        out_t1.triangle[1] = *inside_points[1];
        out_t1.triangle[2] = intersect1;

        out_t2.triangle[0] = *inside_points[1];
        out_t2.triangle[1] = intersect2;
        out_t2.triangle[2] = intersect1;

        return 2;
    }

    return 0;
}

Vec3 Triangle::toCamera(const Vec3& camera_pos) const
{
    return camera_pos - triangle[0];
}
