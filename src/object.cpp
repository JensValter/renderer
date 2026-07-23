#include "object.h"
#include <fstream>
#include <sstream>
#include <string>

// Parses obj file. Normals are ignored since the program generates its own.
bool Object::loadObjectFromFile(const std::string &fileName) {
  std::ifstream stream(fileName);
  if (!stream.is_open()) {
    return false;
  }
  std::string line;
  std::vector<Vec4> verticies;
  std::vector<Vec2> textureCoordinates;
  while (std::getline(stream, line)) {
    std::stringstream ss(line);
    std::string type;
    ss >> type;
    if (type == "v") {
      float x, y, z;
      ss >> x >> y >> z;
      verticies.push_back({x, y, z});
    }
    if (type == "vt") {
      float x, y;
      ss >> x >> y;
      textureCoordinates.push_back({x, y});
    }
    if (type == "f") {
      std::string v1, v2, v3;
      ss >> v1 >> v2 >> v3;
      auto p1 = vtFieldSeperator(v1);
      auto p2 = vtFieldSeperator(v2);
      auto p3 = vtFieldSeperator(v3);

      Vec2 tex1 = (p1.second != -1) ? textureCoordinates[p1.second - 1] : Vec2{0, 0};
      Vec2 tex2 = (p2.second != -1) ? textureCoordinates[p2.second - 1] : Vec2{0, 0};
      Vec2 tex3 = (p3.second != -1) ? textureCoordinates[p3.second - 1] : Vec2{0, 0};

      m_triangles.push_back({verticies[p1.first - 1], verticies[p2.first - 1], verticies[p3.first - 1],
                             tex1, tex2, tex3});
  }
  }
  return true;
}
std::pair<int, int> Object::vtFieldSeperator(std::string &field) {
  auto index = field.find_first_of('/');
  // only vertex coordinate is given
  if (index == std::string::npos)
    return {std::stoi(field), -1};
  // vertex and texture coordinates are given; if a normal is specified it is
  // ignored
  else if (field.substr(index, 2) != "//") {
    auto index2 = field.find_first_of('/', index + 1);
    if (index2 == std::string::npos)
      return {std::stoi(field.substr(0, index)),
              std::stoi(field.substr(index + 1))};
    else
      return {std::stoi(field.substr(0, index)),
              std::stoi(field.substr(index + 1, index2 - index - 1))};
  }
  // vertex and normal are given, normal is ignored
  else
    return {std::stoi(field.substr(0, index)), -1};
}
