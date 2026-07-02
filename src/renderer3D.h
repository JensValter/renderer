#pragma once
#include <vector>
#include <stdint.h>
#include "mat4x4.h"
#include "rasterMath.h"
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

    void drawPixel(int x, int y, float z, uint32_t color);
    void drawLine(RasterVertex a, RasterVertex b, uint32_t color);
    void drawHorizontalLine(int x0, int x1, int y, float z0, float z1, uint32_t color);

    void drawFlatBottom(RasterVertex v1, RasterVertex v2, RasterVertex v3, uint32_t color);
    void drawFlatTop(RasterVertex v1, RasterVertex v2, RasterVertex v3, uint32_t color);

    void planeClipping(const Vec3 &p0, const Vec3 &n, Triangle &t, std::vector<Triangle>& out);
    void ndcClipping(Triangle t,std::vector<Triangle>& out);

    void drawTriangle(const Triangle& triangle,const Mat4x4& model, const Mat4x4& view, const Mat4x4& proj,
                   const Vec3& camPos, const Vec3& lightDir, int width, int height);        

    void drawTriangleOutline(RasterVertex v1, RasterVertex v2, RasterVertex v3, uint32_t color);
    void drawTriangleFill(RasterVertex v1, RasterVertex v2, RasterVertex v3, uint32_t color);
};
