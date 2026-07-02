#pragma once

struct Vec3
{
    float x = 0.0f;
    float y = 0.0f;
    float z = 0.0f;

    Vec3 operator+(const Vec3& v) const;
    Vec3 operator-(const Vec3& v) const;
    Vec3 operator*(float value) const;
    Vec3 operator *= (float value);
    static Vec3 vecPlanIntersect(const Vec3 & p0, const Vec3 & n, const Vec3 &start, const Vec3 & end);
    Vec3& operator+=(const Vec3& v);
    Vec3& operator-=(const Vec3& v);
    

    float dotProduct(const Vec3& v) const;
    Vec3 crossProduct(const Vec3& v) const;

    static float distanceToPlane(const Vec3& normal, const Vec3 &p, const Vec3& p0);


    void normalizeVector();
    
};
