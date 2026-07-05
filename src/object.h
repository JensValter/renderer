#pragma once
#include "vector"
#include "string"
#include "triangle.h"

class Object{
    private:
        std::pair<int,int> vtFieldSeperator(std::string& field);
    public:
    std::vector<Triangle> m_triangles;
    bool loadObjectFromFile(const std::string& fileName);

};
