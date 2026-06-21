#include "object.h"
#include <fstream>
#include <sstream>


bool Object::loadObjectFromFile(const std::string& fileName)
{
    std::ifstream stream(fileName);
    if(!stream.is_open()){
           return false;

    }
     
    std::string line;
    std::vector<Vec3> verticies;

    while(std::getline(stream,line)){
        std::stringstream ss(line);
        std::string type;
        ss>>type;
        if(type == "v"){
            float x,y,z;
            ss >> x >> y >> z;
            verticies.push_back({x,y,z});
        }

        if(type == "f"){
            int v1,v2,v3;
            ss >> v1 >> v2 >> v3;
            m_triangles.push_back({verticies[v1-1], verticies[v2 - 1], verticies[v3 -1]});
        }

    }

    return true;
}