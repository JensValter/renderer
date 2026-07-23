#pragma once

struct Vec4
{
    float x = 0.0f;
    float y = 0.0f;
    float z = 0.0f;
    float w = 0.0f;

    Vec4 operator+(const Vec4& v) const;
    Vec4 operator-(const Vec4& v) const;
    Vec4 operator*(float value) const;
    Vec4 operator *= (float value);
    static Vec4 vecPlanIntersect(const Vec4 & p0, const Vec4 & n, const Vec4 &start, const Vec4 & end, float &t);
    Vec4& operator+=(const Vec4& v);
    Vec4& operator-=(const Vec4& v);
    

    float dotProduct(const Vec4& v) const;
    Vec4 crossProduct(const Vec4& v) const;

    static float distanceToPlane(const Vec4& normal, const Vec4& p, const Vec4& p0);


    void normalizeVector();
    
};
