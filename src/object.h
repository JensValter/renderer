#pragma once
#include "vector"
#include "string"
#include "triangle.h"

class Object{
    public:
    std::vector<Triangle> m_triangles;
    bool loadObjectFromFile(const std::string& fileName);

};