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

struct Line2D
{
    Line2D(const Vec2& p1, const Vec2& p2, const Color& color) : p1(p1), p2(p2), color(color) {}
    Vec2 p1;
    Vec2 p2;
    Color color;
};

img::EasyImage drawFigures(std::vector<Mesh>& figures, const Color& background, uint32_t size, double d);
img::EasyImage drawLines(const std::forward_list<Line2D>& lines, const Color& background, int size);



#endif //ENGINE_LINE_H
