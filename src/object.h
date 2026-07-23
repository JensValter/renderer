#pragma once
#include "vector"
#include "string"
#include "triangle.h"
#include "texture.h"

class Object{
    private:
        std::pair<int,int> vtFieldSeperator(std::string& field);
    public:
    std::vector<Triangle> m_triangles;
    Texture texture;
    bool loadObjectFromFile(const std::string& fileName);

};
