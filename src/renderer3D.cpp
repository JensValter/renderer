#include "renderer3D.h"
#include <algorithm>

Renderer3D::Renderer3D(Window& window): m_window(window){
    m_depthBuffer.resize(window.height* window.width);
}

void Renderer3D::clear(uint32_t color){
  for(int i = 0; i<(m_window.height * m_window.width); i++){
        m_window.buffer[i] = color;
        m_depthBuffer[i] = 1000.0f;
  }
}

void Renderer3D::drawPixel(int x, int y, float z, uint32_t color){
    if(x<0 || x >= m_window.width)
        return;

    if(y<0 || y >= m_window.height)
        return;


    int index = y* m_window.width + x;

    if(z < m_depthBuffer[index]){
        m_window.buffer[index] = color;
        m_depthBuffer[index] = z;
    }

}

void Renderer3D::drawLine(RasterVertex a, RasterVertex b, uint32_t color)
{
    int x0 = a.x;
    int y0 = a.y;
    float z0 = a.z;
    int x1 = b.x;
    int y1 = b.y;
    float z1 = b.z;
    

    int dx = std::abs(x1 - x0);
    int dy = std::abs(y1 - y0);

    int stepX = x0 < x1 ? 1 : -1;
    int stepY = y0 < y1 ? 1 : -1;
    float stepZ = z0 < z1 ?1.0f :-1.0f;

    int error = dx - dy;

    while (true)
    {
        drawPixel(x0, y0, 0.0f, color);

        if (x0 == x1 && y0 == y1)
            break;

        int error2 = error * 2;

        if (error2 > -dy)
        {
            error -= dy;
            x0 += stepX;
        }

        if (error2 < dx)
        {
            error += dx;
            y0 += stepY;
        }
    }
}

void Renderer3D::drawHorizontalLine(int x0, int x1, int y, float z0, float z1, uint32_t color){
    if (x0 == x1) {
        drawPixel(x0, y, z0, color);
        return;
    }

    float dz = (z1 - z0) / (float)(x1 - x0);
    float z = z0;

    for(int i = x0; i <= x1; i++){
        drawPixel(i, y, z, color);
        z += dz;
    }
}

void Renderer3D::drawTriangle(RasterVertex v1, RasterVertex v2, RasterVertex v3, uint32_t color)
{

    drawLine(v1,v2,color);
    drawLine(v2,v3,color);
    drawLine(v3,v1,color);
    
}

void Renderer3D::drawFlatBottom(RasterVertex v1, RasterVertex v2, RasterVertex v3, uint32_t color)
{
    float dx_left  = (float)(v2.x - v1.x) / (float)(v2.y - v1.y);
    float dx_right = (float)(v3.x - v1.x) / (float)(v3.y - v1.y);

    float dz_left  = (v2.z - v1.z) / (float)(v2.y - v1.y);
    float dz_right = (v3.z - v1.z) / (float)(v3.y - v1.y);

    float x_left = v1.x, x_right = v1.x;
    float z_left = v1.z, z_right = v1.z;

    for(int line = v1.y; line <= v2.y; line++){
        if (x_left <= x_right)
            drawHorizontalLine((int)x_left, (int)x_right, line, z_left, z_right, color);
        else
            drawHorizontalLine((int)x_right, (int)x_left, line, z_right, z_left, color);

        x_left += dx_left;
        x_right += dx_right;
        z_left += dz_left;
        z_right += dz_right;
    }
}

void Renderer3D::drawFlatTop(RasterVertex v1, RasterVertex v2, RasterVertex v3, uint32_t color)
{
    float dx_left  = (float)(v3.x - v1.x) / (float)(v3.y - v1.y);
    float dx_right = (float)(v3.x - v2.x) / (float)(v3.y - v2.y);

    float dz_left  = (v3.z - v1.z) / (float)(v3.y - v1.y);
    float dz_right = (v3.z - v2.z) / (float)(v3.y - v2.y);

    float x_left = v3.x, x_right = v3.x;
    float z_left = v3.z, z_right = v3.z;

    for(int line = v3.y; line >= v1.y; line--){
        if (x_left <= x_right)
            drawHorizontalLine((int)x_left, (int)x_right, line, z_left, z_right, color);
        else
            drawHorizontalLine((int)x_right, (int)x_left, line, z_right, z_left, color);

        x_left -= dx_left;
        x_right -= dx_right;
        z_left -= dz_left;
        z_right -= dz_right;
    }
}

void Renderer3D::drawTriangleFill(RasterVertex v1, RasterVertex v2, RasterVertex v3, uint32_t color)
{
    if (v2.y < v1.y) std::swap(v1, v2);
    if (v3.y < v1.y) std::swap(v1, v3);
    if (v3.y < v2.y) std::swap(v2, v3);

    if (v1.y == v3.y)
        return;

    if(v2.y == v3.y)
        drawFlatBottom(v1,v2,v3,color);

    else if (v1.y == v2.y)
        drawFlatTop(v1,v2,v3,color);
    
    else{
        float t = (float)(v2.y - v1.y) / (float)(v3.y - v1.y);
        RasterVertex v4 = {(int)(v1.x + t * (v3.x - v1.x)), v2.y, v1.z + t * (v3.z - v1.z)};
        drawFlatBottom(v1,v2,v4,color);
        drawFlatTop(v2,v4,v3,color);
    }
}
void Renderer3D::present(){
    m_window.draw();   
}