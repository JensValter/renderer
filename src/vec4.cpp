#include "vec4.h"
#include <cmath>

Vec4 Vec4::operator+(const Vec4& v) const
{
    return {
        x + v.x,
        y + v.y,
        z + v.z,
        w + v.w
    };
}

Vec4 Vec4::operator-(const Vec4& v) const
{
    return {
        x - v.x,
        y - v.y,
        z - v.z,
        w - v.w
    };
}

Vec4 Vec4::operator*(float value) const
{
    return {
        x * value,
        y * value,
        z * value,
        w * value
    };
}

Vec4 Vec4::operator*=(float value)
{
    x *= value;
    y *= value;
    z *= value;
    w *= value;
    return *this;
}

Vec4 Vec4::vecPlanIntersect(const Vec4 &p0, const Vec4 &n, const Vec4 &start, const Vec4 &end, float& t)
{
    Vec4 v = end - start;
    float dp = n.dotProduct(v);
    if (fabs(dp) < 0.0001f)
        return start;
    t = (-n.dotProduct(start - p0)) / dp;
    return start + v * t;
}

float Vec4::dotProduct(const Vec4& v) const
{
    return x * v.x + y * v.y + z * v.z;
}

Vec4 Vec4::crossProduct(const Vec4& v) const
{
    return {
        (y * v.z) - (z * v.y),
        (z * v.x) - (x * v.z),
        (x * v.y) - (y * v.x),
        0.0f
    };
}

float Vec4::distanceToPlane(const Vec4& normal, const Vec4 &p, const Vec4 &p0)
{
    return normal.dotProduct(p - p0);
}

void Vec4::normalizeVector()
{
    float length = sqrtf(x * x + y * y + z * z);
    if (length == 0.0f)
        return;
    float f = 1.0f / length;
    x *= f;
    y *= f;
    z *= f;
}

Vec4& Vec4::operator+=(const Vec4& v)
{
    x += v.x;
    y += v.y;
    z += v.z;
    w += v.w;
    return *this;
}

Vec4& Vec4::operator-=(const Vec4& v)
{
    x -= v.x;
    y -= v.y;
    z -= v.z;
    w -= v.w;
    return *this;
}
