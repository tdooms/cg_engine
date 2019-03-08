/*
 * @summary: A file with all definitions of simple structs and basic operations on them
 * @author: Thomas Dooms
 * @date: 22/02/2019
 * @version: 1.0
 */

#ifndef ENGINE_PRIMITIVES_H
#define ENGINE_PRIMITIVES_H

#include <forward_list>
#include "easy_image.h"
#include "ini_configuration.h"

#include "math/mat4.h"
#include "math/vec4.h"
#include "math/color.h"

struct Line2D
{
    Line2D(const Vec2& p1, const Vec2& p2, const Color& color) : p1(p1), p2(p2), color(color) {}
    Vec2 p1;
    Vec2 p2;
    Color color;
};

struct Figure3D
{
    Figure3D(uint32_t numVertices, uint32_t numIndices, Color color) : vertices(numVertices), indices(numIndices), color(color) {}

    std::vector<Vec3> vertices;
    std::vector<std::array<uint32_t, 2>> indices;
    Color color;
};

typedef std::forward_list<Line2D> Lines2D;
typedef std::forward_list<Figure3D> Figures3D;

img::EasyImage draw2DLines(const Lines2D& lines, const Color& background, int size);

Figure3D parseFigure(const ini::Section& section);
Lines2D doProjection(Figures3D& figures, const Mat4& matrix, double d);


#endif //ENGINE_PRIMITIVES_H
