
#include "renderer3D.h"
#include "rasterMath.h"
#include "texture.h"
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

void Renderer3D::drawPixel(int x, int y, float z, uint32_t color, float brightness) {
    if (x < 0 || x >= m_width || y < 0 || y >= m_height)
        return;

    int index = y * m_width + x;

    if (z < m_depthBuffer[index]) {
        m_pixels[index] = brightnessModifier(brightness, color);
        m_depthBuffer[index] = z;
    }
}

void Renderer3D::drawLine(Vec4 a, Vec4 b, uint32_t color)
{
    int x0 = (int)a.x;
    int y0 = (int)a.y;
    int x1 = (int)b.x;
    int y1 = (int)b.y;

    int dx = std::abs(x1 - x0);
    int dy = std::abs(y1 - y0);

    int stepX = x0 < x1 ? 1 : -1;
    int stepY = y0 < y1 ? 1 : -1;

    int error = dx - dy;

    while (true)
    {
        drawPixel(x0, y0, 0.0f, color, 1.0f);

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

void Renderer3D::drawHorizontalLine(int x0, int x1, int y, float z0, float z1, float u0, float u1, float v0, float v1, float w0, float w1, float brightness, const Texture& texture) {

    uint32_t color;
    int texX;
    int texY;
    if (x0 == x1) {
        float invW = w0;
        float u = u0 / invW;
        float v = v0 / invW;
        texX = (int)(u * texture.width);
        texY = (int)(v * texture.height);
        color = texture.texBuffer[texY * texture.width + texX];
        drawPixel(x0, y, z0, color, brightness);
        return;
    }

    float lineLength = (float)(x1 - x0);
    float dz = (z1 - z0) / lineLength;
    float du = (u1 - u0) / lineLength;
    float dv = (v1 - v0) / lineLength;
    float dw = (w1 - w0) / lineLength;

    for (int i = x0; i <= x1; i++) {
        float invW = w0;
        float u = u0 / invW;
        float v = v0 / invW;
        texX = (int)(u * texture.width);
        texY = (int)(v * texture.height);
        color = texture.texBuffer[texY * texture.width + texX];
        drawPixel(i, y, z0, color,brightness);

        z0 += dz;
        u0 += du;
        v0 += dv;
        w0 += dw;
    }
}

void Renderer3D::drawTriangleOutline(const Triangle& t, uint32_t color) {
    drawLine(t.triangle[0], t.triangle[1], color);
    drawLine(t.triangle[1], t.triangle[2], color);
    drawLine(t.triangle[2], t.triangle[0], color);
}


void Renderer3D::drawTriangle(const Triangle& triangle, const Mat4x4& model, const Mat4x4& view, const Mat4x4& proj,
                               const Vec4& camPos, const Vec4& lightDir, const Texture& texture, int width, int height)
{
    Triangle tri = triangle;
    tri.color = 0xFFFFFFFF;

    tri.matrixMultiply(model);
    
    Vec4 normal = tri.normal();
    normal.normalizeVector();
    if (normal.dotProduct(tri.toCamera(camPos)) <= 0.0f)
        return;

    float dotP = normal.dotProduct(lightDir);
    tri.matrixMultiply(view);

    m_nearPlaneClipped.clear();
    planeClipping({0.0f, 0.0f, 0.1f}, {0.0f, 0.0f, 1.0f}, tri, m_nearPlaneClipped);

    m_ndcClipped.clear();
    for (Triangle nearTri : m_nearPlaneClipped)
    {
        nearTri.applyTransformation(proj);
        ndcClipping(nearTri, m_ndcClipped);
    }

    for (Triangle& clippedTri : m_ndcClipped)
    {
        ndcToScreen(clippedTri, width, height);
        clippedTri.brightness = dotP;
        drawTriangleFill(clippedTri, texture);
    }
}

void Renderer3D::drawFlatBottom(Triangle& tri, const Texture& texture) {
    Vec4& v1 = tri.triangle[0];
    Vec4& v2 = tri.triangle[1];
    Vec4& v3 = tri.triangle[2];
    Vec2& t1 = tri.tex[0];
    Vec2& t2 = tri.tex[1];
    Vec2& t3 = tri.tex[2];

    float dy = v2.y - v1.y;
    if (dy == 0) return;

    float dx_left  = (v2.x - v1.x) / dy;
    float dx_right = (v3.x - v1.x) / dy;
    float dz_left  = (v2.z - v1.z) / dy;
    float dz_right = (v3.z - v1.z) / dy;

    float du_left  = (t2.u - t1.u) / dy;
    float du_right = (t3.u - t1.u) / dy;
    float dv_left  = (t2.v - t1.v) / dy;
    float dv_right = (t3.v - t1.v) / dy;

    float dw_left  = (v2.w - v1.w) / dy;
    float dw_right = (v3.w - v1.w) / dy;

    float x_left = v1.x, x_right = v1.x;
    float z_left = v1.z, z_right = v1.z;
    float u_left = t1.u, u_right = t1.u;
    float v_left = t1.v, v_right = t1.v;
    float w_left = v1.w, w_right = v1.w;

    for (int line = (int)v1.y; line <= (int)v2.y; line++) {
        if (x_left <= x_right)
            drawHorizontalLine((int)x_left, (int)x_right, line, z_left, z_right, u_left, u_right, v_left, v_right, w_left, w_right, tri.brightness, texture);
        else
            drawHorizontalLine((int)x_right, (int)x_left, line, z_right, z_left, u_right, u_left, v_right, v_left, w_right, w_left, tri.brightness, texture);

        x_left += dx_left;
        x_right += dx_right;
        z_left += dz_left;
        z_right += dz_right;
        u_left += du_left;
        u_right += du_right;
        v_left += dv_left;
        v_right += dv_right;
        w_left += dw_left;
        w_right += dw_right;
    }
}

void Renderer3D::drawFlatTop(Triangle& tri, const Texture& texture) {
    Vec4& v1 = tri.triangle[0];
    Vec4& v2 = tri.triangle[1];
    Vec4& v3 = tri.triangle[2];

    Vec2& t1 = tri.tex[0];
    Vec2& t2 = tri.tex[1];
    Vec2& t3 = tri.tex[2];

    float dy1 = v3.y - v1.y;
    float dy2 = v3.y - v2.y;
    if (dy1 == 0 || dy2 == 0) return;

    float dx_left  = (v3.x - v1.x) / dy1;
    float dx_right = (v3.x - v2.x) / dy2;

    float dz_left  = (v3.z - v1.z) / dy1;
    float dz_right = (v3.z - v2.z) / dy2;

    float du_left  = (t3.u - t1.u) / dy1;
    float du_right = (t3.u - t2.u) / dy2;

    float dv_left  = (t3.v - t1.v) / dy1;
    float dv_right = (t3.v - t2.v) / dy2;

    float dw_left  = (v3.w - v1.w) / dy1;
    float dw_right = (v3.w - v2.w) / dy2;

    float x_left = v3.x, x_right = v3.x;
    float z_left = v3.z, z_right = v3.z;
    float u_left = t3.u, u_right = t3.u;
    float v_left = t3.v, v_right = t3.v;
    float w_left = v3.w, w_right = v3.w;

    for (int line = (int)v3.y; line >= (int)v1.y; line--) {
        if (x_left <= x_right)
            drawHorizontalLine((int)x_left, (int)x_right, line, z_left, z_right, u_left, u_right, v_left, v_right, w_left, w_right, tri.brightness, texture);
        else
            drawHorizontalLine((int)x_right, (int)x_left, line, z_right, z_left, u_right, u_left, v_right, v_left, w_right, w_left, tri.brightness, texture);

        x_left -= dx_left;
        x_right -= dx_right;
        z_left -= dz_left;
        z_right -= dz_right;
        u_left -= du_left;
        u_right -= du_right;
        v_left -= dv_left;
        v_right -= dv_right;
        w_left -= dw_left;
        w_right -= dw_right;
    }
}

void Renderer3D::drawTriangleFill(Triangle& tri, const Texture& texture) {
    Vec4& v1 = tri.triangle[0];
    Vec4& v2 = tri.triangle[1];
    Vec4& v3 = tri.triangle[2];
    Vec2& u1 = tri.tex[0];
    Vec2& u2 = tri.tex[1];
    Vec2& u3 = tri.tex[2];

    if (v2.y < v1.y)
        tri.vertexSwap(0, 1);
    if (v3.y < v1.y)
        tri.vertexSwap(0, 2);
    if (v3.y < v2.y)
        tri.vertexSwap(1, 2);

    if (v1.y == v3.y)
        return;

    if (v2.y == v3.y) {
        drawFlatBottom(tri, texture);
    }
    else if (v1.y == v2.y) {
        drawFlatTop(tri, texture);
    }
    else {
        float t = (v2.y - v1.y) / (v3.y - v1.y);
        Vec4 v4 = { v1.x + t * (v3.x - v1.x), v2.y, v1.z + t * (v3.z - v1.z), v1.w + t * (v3.w - v1.w) };

        Vec2 uv4 = { u1.u + t * (u3.u - u1.u), u1.v + t * (u3.v - u1.v) };

        Triangle top;
        top.triangle[0] = v1;
        top.triangle[1] = v2;
        top.triangle[2] = v4;
        top.tex[0] = u1;
        top.tex[1] = u2;
        top.tex[2] = uv4;
        top.brightness = tri.brightness;

        Triangle bottom;
        bottom.triangle[0] = v2;
        bottom.triangle[1] = v4;
        bottom.triangle[2] = v3;
        bottom.tex[0] = u2;
        bottom.tex[1] = uv4;
        bottom.tex[2] = u3;
        bottom.brightness = tri.brightness;

        drawFlatBottom(top, texture);
        drawFlatTop(bottom, texture);
    }
}

void Renderer3D::ndcClipping(Triangle t, std::vector<Triangle>& out)
{
    m_clipInput.clear();
    m_clipInput.push_back(t);

    for (int i = 0; i < 4; i++)
    {
        Vec4 n;
        switch(i){
            case 0: n = {1.0f, 0.0f, 0.0f}; break;
            case 1: n = {-1.0f, 0.0f, 0.0f}; break;
            case 2: n = {0.0f, 1.0f, 0.0f}; break;
            case 3: n = {0.0f, -1.0f, 0.0f}; break;
        }

        m_clipOutput.clear();
        for (Triangle& tri : m_clipInput)
            planeClipping(n * (-1.0f), n, tri, m_clipOutput);

        std::swap(m_clipInput, m_clipOutput);
    }

    out.insert(out.end(), m_clipInput.begin(), m_clipInput.end());
}

void Renderer3D::planeClipping(const Vec4 &p0, const Vec4 &n, Triangle &t, std::vector<Triangle>& out)
{   
    Vec4 normal = n;
    normal.normalizeVector();

    Vec4* inside_points[3]; 
    int insidePointCount = 0;
    Vec4* outside_points[3];
    int outsidePointsCount = 0;
    Vec2* inside_tex_points[3];
    int insideTexPointCount = 0;
    Vec2* outside_tex_points[3];
    int outsideTexPointCount = 0;

   for(int i = 0; i<3; i++){
        float dist = Vec4::distanceToPlane(normal, t.triangle[i], p0);
        if(dist >= 0){
            inside_points[insidePointCount++] = &t.triangle[i];
            inside_tex_points[insideTexPointCount++] = &t.tex[i];
        }
        else{
            outside_points[outsidePointsCount++] = &t.triangle[i];
            outside_tex_points[outsideTexPointCount++] = &t.tex[i];
        }
    }

    if(insidePointCount == 0);
    else if(insidePointCount == 3){
        out.push_back(t);
    }

    else if(insidePointCount == 1){
        float t1;
        Vec4 intersect1 = Vec4::vecPlanIntersect(p0, normal, *inside_points[0], *outside_points[0], t1);
        float t2;
        Vec4 intersect2 = Vec4::vecPlanIntersect(p0, normal, *inside_points[0], *outside_points[1], t2);

        Vec2 texIntersect1 = {
            t1 * (outside_tex_points[0]->u - inside_tex_points[0]->u) + inside_tex_points[0]->u,
            t1 * (outside_tex_points[0]->v - inside_tex_points[0]->v) + inside_tex_points[0]->v
        };
        Vec2 texIntersect2 = {
            t2 * (outside_tex_points[1]->u - inside_tex_points[0]->u) + inside_tex_points[0]->u,
            t2 * (outside_tex_points[1]->v - inside_tex_points[0]->v) + inside_tex_points[0]->v
        };

        out.push_back({*inside_points[0], intersect1, intersect2,
                       *inside_tex_points[0], texIntersect1, texIntersect2});
    }
    else if(insidePointCount == 2){
        float t1;
        Vec4 intersect1 = Vec4::vecPlanIntersect(p0, normal, *inside_points[0], *outside_points[0], t1);
        float t2;
        Vec4 intersect2 = Vec4::vecPlanIntersect(p0, normal, *inside_points[1], *outside_points[0], t2);

        Vec2 texIntersect1 = {
            t1 * (outside_tex_points[0]->u - inside_tex_points[0]->u) + inside_tex_points[0]->u,
            t1 * (outside_tex_points[0]->v - inside_tex_points[0]->v) + inside_tex_points[0]->v
        };
        Vec2 texIntersect2 = {
            t2 * (outside_tex_points[0]->u - inside_tex_points[1]->u) + inside_tex_points[1]->u,
            t2 * (outside_tex_points[0]->v - inside_tex_points[1]->v) + inside_tex_points[1]->v
        };

        out.push_back({*inside_points[0], *inside_points[1], intersect1,
                       *inside_tex_points[0], *inside_tex_points[1], texIntersect1});
        out.push_back({*inside_points[1], intersect2, intersect1,
                       *inside_tex_points[1], texIntersect2, texIntersect1});
    }
}
