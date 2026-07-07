#pragma once
#include <vector>
#include <stdint.h>
#include "mat4x4.h"
#include "texture.h"
#include "triangle.h"

struct RenderBuffer;

class Renderer3D
{
private:
    int m_width;
    int m_height;
    uint32_t* m_pixels;
    std::vector<float> m_depthBuffer;
    std::vector<Triangle> m_clipInput; 
    std::vector<Triangle> m_clipOutput; 
    std::vector<Triangle> m_nearPlaneClipped;
    std::vector<Triangle> m_ndcClipped;
    
public:
    Renderer3D(RenderBuffer target);
  
    
    void clear(uint32_t color);
    void drawPixel(int x, int y, float z, uint32_t color, float brightness);
    void drawLine(Vec3 a, Vec3 b, uint32_t color);
    void drawHorizontalLine(int x0, int x1, int y, float z0, float z1,
                             float u0, float u1, float v0, float v1, float brightness, const Texture& texture);
    void drawFlatBottom(Triangle &tri, const Texture& texture);
    void drawFlatTop(Triangle &tri, const Texture& texure);
    void planeClipping(const Vec3 &p0, const Vec3 &n, Triangle &t, std::vector<Triangle>& out);
    void ndcClipping(Triangle t,std::vector<Triangle>& out);
    void drawTriangle(const Triangle& triangle,const Mat4x4& model, const Mat4x4& view, const Mat4x4& proj,
                   const Vec3& camPos, const Vec3& lightDir, const Texture& texture, int width, int height);        
    void drawTriangleOutline(const Triangle& t, uint32_t color);
    void drawTriangleFill(Triangle& t, const Texture& tex);
};
