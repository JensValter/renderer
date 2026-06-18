#pragma once
#include "vector"
#include "string"
#include "triangle.h"

class Object{
    public:
    std::vector<Triangle> m_triangles;
    bool loadObjectFromFile(std::string fileName);

};