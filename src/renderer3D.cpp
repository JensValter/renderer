#include "renderer3D.h"
#include "rasterMath.h"
#include "triangle.h"
#include "window.h"
#include <algorithm>
#include <cmath>

Renderer3D::Renderer3D(RenderBuffer target) 
    : m_width(target.width), m_height(target.height), m_pixels(target.buffer) 
{
    m_depthBuffer.resize(m_width * m_height);
    m_clipInput.reserve(16);
    m_clipOutput.reserve(16);
}

void Renderer3D::clear(uint32_t color) {
    for (int i = 0; i < (m_height * m_width); i++) {
        m_pixels[i] = color;
        m_depthBuffer[i] = 1000.0f;
    }
}

void Renderer3D::drawPixel(int x, int y, float z, uint32_t color) {
    if (x < 0 || x >= m_width || y < 0 || y >= m_height)
        return;

    int index = y * m_width + x;

    if (z < m_depthBuffer[index]) {
        m_pixels[index] = color;
        m_depthBuffer[index] = z;
    }
}

void Renderer3D::drawLine(RasterVertex a, RasterVertex b, uint32_t color)
{
    int x0 = a.x;
    int y0 = a.y;
    int x1 = b.x;
    int y1 = b.y;

    int dx = std::abs(x1 - x0);
    int dy = std::abs(y1 - y0);

    int stepX = x0 < x1 ? 1 : -1;
    int stepY = y0 < y1 ? 1 : -1;

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

void Renderer3D::drawHorizontalLine(int x0, int x1, int y, float z0, float z1, uint32_t color) {
    if (x0 == x1) {
        drawPixel(x0, y, z0, color);
        return;
    }

    float dz = (z1 - z0) / (float)(x1 - x0);
    float z = z0;

    for (int i = x0; i <= x1; i++) {
        drawPixel(i, y, z, color);
        z += dz;
    }
}

void Renderer3D::drawTriangleOutline(RasterVertex v1, RasterVertex v2, RasterVertex v3, uint32_t color) {
    drawLine(v1, v2, color);
    drawLine(v2, v3, color);
    drawLine(v3, v1, color);
}


void Renderer3D::drawTriangle(const Triangle& triangle, const Mat4x4& model, const Mat4x4& view, const Mat4x4& proj,
                               const Vec3& camPos, const Vec3& lightDir, int width, int height)
{
    //from local to world coordinates
    Triangle tri = triangle;
    tri.color = 0xFFFFFFFF;

    tri.matrixMultiply(model);
    
    // backface culling
    Vec3 normal = tri.normal();
    normal.normalizeVector();
    if (normal.dotProduct(tri.toCamera(camPos)) <= 0.0f)
        return;

    //calculate brightness based on light direction
    float dotP = normal.dotProduct(lightDir);
    tri.matrixMultiply(view);

    // near plane clipping
    m_nearPlaneClipped.clear();
    planeClipping({0.0f, 0.0f, 0.1f}, {0.0f, 0.0f, 1.0f}, tri, m_nearPlaneClipped);

    // screen edge clipping
    m_ndcClipped.clear();
    for (Triangle nearTri : m_nearPlaneClipped)
    {
        nearTri.applyTransformation(proj);
        ndcClipping(nearTri, m_ndcClipped);
    }

    for (const Triangle& clippedTri : m_ndcClipped)
    {
        RasterVertex p0 = ndcToScreen(clippedTri.triangle[0], width, height);
        RasterVertex p1 = ndcToScreen(clippedTri.triangle[1], width, height);
        RasterVertex p2 = ndcToScreen(clippedTri.triangle[2], width, height);
        drawTriangleFill(p0, p1, p2, brightnessModifier(dotP,tri.color));
    }
}

void Renderer3D::drawFlatBottom(RasterVertex v1, RasterVertex v2, RasterVertex v3, uint32_t color) {
    float dx_left  = (float)(v2.x - v1.x) / (float)(v2.y - v1.y);
    float dx_right = (float)(v3.x - v1.x) / (float)(v3.y - v1.y);

    float dz_left  = (v2.z - v1.z) / (float)(v2.y - v1.y);
    float dz_right = (v3.z - v1.z) / (float)(v3.y - v1.y);

    float x_left = v1.x, x_right = v1.x;
    float z_left = v1.z, z_right = v1.z;

    for (int line = v1.y; line <= v2.y; line++) {
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

void Renderer3D::drawFlatTop(RasterVertex v1, RasterVertex v2, RasterVertex v3, uint32_t color) {
    float dx_left  = (float)(v3.x - v1.x) / (float)(v3.y - v1.y);
    float dx_right = (float)(v3.x - v2.x) / (float)(v3.y - v2.y);

    float dz_left  = (v3.z - v1.z) / (float)(v3.y - v1.y);
    float dz_right = (v3.z - v2.z) / (float)(v3.y - v2.y);

    float x_left = v3.x, x_right = v3.x;
    float z_left = v3.z, z_right = v3.z;

    for (int line = v3.y; line >= v1.y; line--) {
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



//important to keep number of heap allocations low!!
void Renderer3D::ndcClipping(Triangle t, std::vector<Triangle>& out)
{
    m_clipInput.clear();
    m_clipInput.push_back(t);

    for (int i = 0; i < 4; i++)
    {
        Vec3 n;
        switch(i){
            case 0: n = {1.0f, 0.0f, 0.0f}; break; // left
            case 1: n = {-1.0f, 0.0f, 0.0f}; break; // right
            case 2: n = {0.0f, 1.0f, 0.0f}; break; // bottom
            case 3: n = {0.0f, -1.0f, 0.0f}; break; //top
        }

        m_clipOutput.clear();
        for (Triangle& tri : m_clipInput)
            planeClipping(n * (-1.0f), n, tri, m_clipOutput);

        std::swap(m_clipInput, m_clipOutput);
    }

    out.insert(out.end(), m_clipInput.begin(), m_clipInput.end());
}

void Renderer3D::planeClipping(const Vec3 &p0, const Vec3 &n, Triangle &t, std::vector<Triangle>& out)
{   
    Vec3 normal = n;
    normal.normalizeVector();

    Vec3* inside_points[3]; 
    int insidePointCount = 0;

    Vec3* outside_points[3];
    int outsidePointsCount = 0;

    for(int i = 0; i<3; i++){
        float dist = Vec3::distanceToPlane(normal,t.triangle[i],p0);
        dist >= 0 ? inside_points[insidePointCount++] = &t.triangle[i] : outside_points[outsidePointsCount++] = &t.triangle[i];
    }

    if(insidePointCount == 0);

    else if(insidePointCount == 3){
        out.push_back(t);
    }

    else if(insidePointCount == 1){
            out.push_back({*inside_points[0], Vec3::vecPlanIntersect(
            p0, normal, *inside_points[0], *outside_points[0]),Vec3::vecPlanIntersect(p0, normal, *inside_points[0], *outside_points[1])});
    }

    else if(insidePointCount == 2){

        Vec3 intersect1 = Vec3::vecPlanIntersect(p0, normal, *inside_points[0], *outside_points[0]);

        Vec3 intersect2 = Vec3::vecPlanIntersect(p0, normal, *inside_points[1], *outside_points[0]);

        out.push_back({*inside_points[0],*inside_points[1], intersect1});
        out.push_back({*inside_points[1], intersect2, intersect1});
    }
}


void Renderer3D::drawTriangleFill(RasterVertex v1, RasterVertex v2, RasterVertex v3, uint32_t color) {
    if (v2.y < v1.y) std::swap(v1, v2);
    if (v3.y < v1.y) std::swap(v1, v3);
    if (v3.y < v2.y) std::swap(v2, v3);

    if (v1.y == v3.y)
        return;

    if (v2.y == v3.y)
        drawFlatBottom(v1, v2, v3, color);
    else if (v1.y == v2.y)
        drawFlatTop(v1, v2, v3, color);
    else {
        float t = (float)(v2.y - v1.y) / (float)(v3.y - v1.y);
        RasterVertex v4 = { (int)(v1.x + t * (v3.x - v1.x)), v2.y, v1.z + t * (v3.z - v1.z) };
        drawFlatBottom(v1, v2, v4, color);
        drawFlatTop(v2, v4, v3, color);
    }
}
