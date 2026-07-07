#pragma once
#include <cstdint>
#include <vector>
struct Texture{
  private:
  public:
  std::vector<uint32_t> texBuffer = {0xffffffff};
  int width = 1;
  int height = 1;  
};
