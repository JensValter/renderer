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


Vec3 Triangle::toCamera(const Vec3& camera_pos) const
{
    return camera_pos - triangle[0];
}

//need to reduce heap allocations, make them static or something
void Triangle::ndcClipping(Triangle t, std::vector<Triangle>& out)
{
    std::vector<Triangle> current;
    std::vector<Triangle> next;
    current.push_back(t);

    for (int i = 0; i < 4; i++)
    {
        Vec3 n;
        switch(i){
            case 0: n = {1.0f, 0.0f, 0.0f}; break; // left
            case 1: n = {-1.0f, 0.0f, 0.0f}; break; // right
            case 2: n = {0.0f, 1.0f, 0.0f}; break; // bottom
            case 3: n = {0.0f, -1.0f, 0.0f}; break; //top
        }

        next.clear();
        for (Triangle& tri : current)
            planeClipping(n * (-1.0f), n, tri, next);

        std::swap(current, next);
    }

    out.insert(out.end(), current.begin(), current.end());
}

void Triangle::planeClipping(const Vec3 &p0, const Vec3 &n, Triangle &t, std::vector<Triangle>& out)
{   
    Vec3 normal = n;
    normal.normalizeVector();

    Vec3* inside_points[3]; 
    int insidePointCount = 0;

    Vec3* outside_points[3];
    int outsidePointsCount = 0;

    for(int i = 0; i<3; i++){
        float dist = Vec3::distanceToPlane(normal,t.triangle[i],p0);
        dist >= 0 ? inside_points[insidePointCount++] = &t.triangle[i] : outside_points[outsidePointsCount++] = &t.triangle[i];
    }

    if(insidePointCount == 0);

    else if(insidePointCount == 3){
        out.push_back(t);
    }

    else if(insidePointCount == 1){
            out.push_back({*inside_points[0], Vec3::vecPlanIntersect(
            p0, normal, *inside_points[0], *outside_points[0]),Vec3::vecPlanIntersect(p0, normal, *inside_points[0], *outside_points[1])});
    }

    else if(insidePointCount == 2){

        Vec3 intersect1 = Vec3::vecPlanIntersect(p0, normal, *inside_points[0], *outside_points[0]);

        Vec3 intersect2 = Vec3::vecPlanIntersect(p0, normal, *inside_points[1], *outside_points[0]);

        out.push_back({*inside_points[0],*inside_points[1], intersect1});
        out.push_back({*inside_points[1], intersect2, intersect1});
    }
}