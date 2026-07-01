#include "vec3.h"

#include <cmath>

Vec3 Vec3::operator+(const Vec3& v) const
{
    return {
        x + v.x,
        y + v.y,
        z + v.z
    };
}

Vec3 Vec3::operator-(const Vec3& v) const
{
    return {
        x - v.x,
        y - v.y,
        z - v.z
    };
}


Vec3 Vec3::operator*(float value) const
{

    return {
        x * value,
        y * value,
        z * value
    };
}

Vec3 Vec3::operator*=(float value)
{
    x *= value;
    y *= value;
    z *= value;
    return *this;
    
}

Vec3 Vec3::vecPlanIntersect(const Vec3 &p0, const Vec3 &n, const Vec3 &start, const Vec3 &end)
{
    Vec3 v = end - start;
    float dp = n.dotProduct(v);
    if(fabs(dp) < 0.0001f)
        return start; // Line is parallel to the plane

    float t = (-n.dotProduct(start - p0)) / dp;

    return start + v * t;

}



float Vec3::dotProduct(const Vec3& v) const
{
    return x * v.x + y * v.y + z * v.z;
}

Vec3 Vec3::crossProduct(const Vec3& v) const
{
    return {
        (y * v.z) - (z * v.y),
        (z * v.x) - (x * v.z),
        (x * v.y) - (y * v.x)
    };
}

float Vec3::distanceToPlane(const Vec3& normal, const Vec3 &p, const Vec3 &p0)
{
    return normal.dotProduct(p-p0);
}

void Vec3::normalizeVector()
{
    float length = sqrtf(x * x + y * y + z * z);

    if (length == 0.0f)
        return;

    float f = 1.0f / length;

    x *= f;
    y *= f;
    z *= f;
}

Vec3& Vec3::operator+=(const Vec3& v)
{
    x += v.x;
    y += v.y;
    z += v.z;
    return *this;
}

Vec3& Vec3::operator-=(const Vec3& v)
{
    x -= v.x;
    y -= v.y;
    z -= v.z;
    return *this;
}



