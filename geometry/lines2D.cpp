//============================================================================
// @name        : line2D.cpp
// @author      : Thomas Dooms
// @date        : 3/8/19
// @version     : 
// @copyright   : BA1 Informatica - Thomas Dooms - University of Antwerp
// @description : 
//============================================================================

#include "lines2D.h"
#include <algorithm>
#include <set>

struct Index
{
    Index(const uint32_t p1, const uint32_t p2) : p1(p1), p2(p2) {}
    uint32_t p1, p2;
};
bool operator<(const Index& a, const Index& b)
{

    if((a.p1 > a.p2) ^ (b.p1 > b.p2))   return a.p2 < b.p1 || (a.p2 >= b.p1 && a.p1 < b.p2);
    else                                return a.p1 < b.p1 || (a.p1 >= b.p1 && a.p2 < b.p2);
}
bool operator>(const Index& a, const Index& b)
{
    if((a.p1 > a.p2) ^ (b.p1 > b.p2))   return a.p2 > b.p1 || (a.p2 <= b.p1 && a.p1 > b.p2);
    else                                return a.p1 > b.p1 || (a.p1 <= b.p1 && a.p2 > b.p2);
}

img::EasyImage drawFigures(std::vector<Mesh>& figures, const Color& background, const uint32_t size, double d)
{
    std::forward_list<Line2D> lines;

    for(int i = figures.size() - 1; i >= 0; i--)
    {
        for (auto& point : figures[i].vertices) point *= -(d/point[2]);  // apply depth division

        std::set<Index> set;
        for (const auto& face : figures[i].indices)
        {
            if(face.size() == 2) set.emplace(face[0], face[1]);
            else
            {
                for(uint32_t j = 0; j < face.size()-1; j++) set.emplace(face[j], face[j+1]);
                set.emplace(face[face.size() - 1], face[0]);
            }
        }
        for(const auto& index : set) lines.emplace_front(figures[i].vertices[index.p1].xy(), figures[i].vertices[index.p2].xy(), figures[i].color);
    }
    return drawLines(lines, background, size);
}

img::EasyImage drawLines(const std::forward_list<Line2D>& lines, const Color& background, const int size)
{
    const auto x0 = std::minmax_element(begin(lines), end(lines), [](const Line2D& a, const Line2D& b){ return a.p1[0] < b.p1[0]; });
    const auto x1 = std::minmax_element(begin(lines), end(lines), [](const Line2D& a, const Line2D& b){ return a.p2[0] < b.p2[0]; });
    const auto y0 = std::minmax_element(begin(lines), end(lines), [](const Line2D& a, const Line2D& b){ return a.p1[1] < b.p1[1]; });
    const auto y1 = std::minmax_element(begin(lines), end(lines), [](const Line2D& a, const Line2D& b){ return a.p2[1] < b.p2[1]; });

    const double xMax = std::max((*x0.second).p1[0], (*x1.second).p2[0]);
    const double xMin = std::min((*x0.first ).p1[0], (*x1.first ).p2[0]);
    const double yMax = std::max((*y0.second).p1[1], (*y1.second).p2[1]);
    const double yMin = std::min((*y0.first ).p1[1], (*y1.first ).p2[1]);

    double xRange = xMax - xMin;
    double yRange = yMax - yMin;

    double width  = size * xRange / std::max(xRange, yRange);
    double height = size * yRange / std::max(xRange, yRange);
    double scale  = 0.95 * width  / xRange;

    double dx = (width  - scale * (xMin+xMax)) / 2.0;
    double dy = (height - scale * (yMin+yMax)) / 2.0;

    img::EasyImage image(width, height, background);
    for(const Line2D& line : lines) image.draw_line(line.p1*scale + Vec2{dx, dy}, line.p2*scale + Vec2{dx, dy}, line.color);

    return image;
}


