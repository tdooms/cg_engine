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

struct Color
{
    double red;
    double green;
    double blue;
    // conversion function, allows casts from Color to img::Color
    explicit operator img::Color() const { return {static_cast<uint8_t>(red*255.99), static_cast<uint8_t>(green*255.99), static_cast<uint8_t>(blue*255.99)}; }
    Color(const std::vector<double>& values) : red(values[0]), green(values[1]), blue(values[2]) {}
};

struct Point2D
{
    double x;
    double y;
};

struct Line2D
{
    Line2D(const Point2D& p1, const Point2D& p2, const Color& color) : p1(p1), p2(p2), color(color) {}
    Point2D p1;
    Point2D p2;
    Color color;
};

typedef std::forward_list<Line2D> Lines2D;

img::EasyImage draw2DLines(const Lines2D& lines, const Color& background, int size);



#endif //ENGINE_PRIMITIVES_H
