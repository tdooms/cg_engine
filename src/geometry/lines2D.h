//============================================================================
// @name        : line2D.h
// @author      : Thomas Dooms
// @date        : 3/8/19
// @version     : 
// @copyright   : BA1 Informatica - Thomas Dooms - University of Antwerp
// @description : 
//============================================================================

#ifndef ENGINE_LINE_H
#define ENGINE_LINE_H

#include <forward_list>

#include "../easy_image.h"
#include "../math/color.h"
#include "../math/vec2.h"
#include "../math/vec3.h"
#include "mesh.h"
#include "zbuffer.h"
#include "light.h"

struct Line2D
{
    Line2D(const Vec3& p1, const Vec3& p2, const Color& color) : p1(p1), p2(p2), color(color) {}

    Vec3 p1;
    Vec3 p2;
    Color color;
};

void drawLine(img::EasyImage& image, const Vec3& p1, const Vec3& p2, const Color& color);
void drawLine(img::EasyImage& image, ZBuffer& buffer, const Vec3& p1, const Vec3& p2, const Color& color);

// tuple<scale, width, height, dx, dy>
std::tuple<double, double, double, double, double> getRanges(const std::vector<Mesh>& meshes, double size, double d);
std::tuple<double, double, double, double, double> getRanges(const std::forward_list<Line2D>& lines, double size);
Vec3 getNormal(const std::vector<Vec3>& triangle);
std::forward_list<Line2D> doProjection(const std::vector<Mesh>& figures, double d);

void calculateLights(std::array<Color, 3>& colors, const Vec3& pos, const Vec3& normal, const Light& light, double reflection, bool directional);
img::Color combineLights(std::array<Color, 3>& colors, const Mesh::Material& material);

void drawTriangle(img::EasyImage& image, ZBuffer& buffer, const Vec3& p1, const Vec3& p2, const Vec3& p3, double d, const Vec2& dxy,
        const Mesh::Material& material, const std::vector<Light>& directional, const std::vector<Light>& point);

img::EasyImage drawTriangulatedMeshes(const std::vector<Mesh>& figures, const Color& background, uint32_t size, const std::vector<Light>& directional, const std::vector<Light>& point);
img::EasyImage drawFigures(const std::vector<Mesh>& figures, const Color& background, uint32_t size, double d, bool depthBuffer);
img::EasyImage drawLines(const std::forward_list<Line2D>& lines, const Color& background, int size, bool depthBuffer);

#endif //ENGINE_LINE_H
