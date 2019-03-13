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

struct Line2D
{
    Line2D(const Vec3& p1, const Vec3& p2, const Color& color) : p1(p1), p2(p2), color(color) {}

    Vec3 p1;
    Vec3 p2;
    Color color;
};

void drawLine(img::EasyImage& image, const Vec3& p1, const Vec3& p2, const Vec3& color);
void drawLine(img::EasyImage& image, ZBuffer& buffer, const Vec3& p1, const Vec3& p2, const Vec3& color);

img::EasyImage drawFigures(std::vector<Mesh>& figures, const Color& background, uint32_t size, double d, bool depthBuffer);
img::EasyImage drawLines(const std::forward_list<Line2D>& lines, const Color& background, int size, bool depthBuffer);

#endif //ENGINE_LINE_H
