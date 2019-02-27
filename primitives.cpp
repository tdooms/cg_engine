#include "primitives.h"
#include "math.h"
#include <limits>
#include <algorithm>

img::EasyImage draw2DLines(const Lines2D& lines, const Color& background, const int size)
{
    const auto x0 = std::minmax_element(begin(lines), end(lines), [](const Line2D& a, const Line2D& b){ return a.p1.x < b.p1.x; });
    const auto x1 = std::minmax_element(begin(lines), end(lines), [](const Line2D& a, const Line2D& b){ return a.p2.x < b.p2.x; });
    const auto y0 = std::minmax_element(begin(lines), end(lines), [](const Line2D& a, const Line2D& b){ return a.p1.y < b.p1.y; });
    const auto y1 = std::minmax_element(begin(lines), end(lines), [](const Line2D& a, const Line2D& b){ return a.p2.y < b.p2.y; });

    const double xMax = std::max((*x0.second).p1.x, (*x1.second).p2.x);
    const double xMin = std::min((*x0.first ).p1.x, (*x1.first ).p2.x);
    const double yMax = std::max((*y0.second).p1.y, (*y1.second).p2.y);
    const double yMin = std::min((*y0.first ).p1.y, (*y1.first ).p2.y);

    double xRange = xMax - xMin;
    double yRange = yMax - yMin;

    double width  = size * xRange / std::max(xRange, yRange);
    double height = size * yRange / std::max(xRange, yRange);
    double scale  = 0.95 * width  / xRange;

    double dx = (width  - scale * (xMin+xMax)) / 2.0;
    double dy = (height - scale * (yMin+yMax)) / 2.0;

    img::EasyImage image(static_cast<uint32_t>(std::round(width)), static_cast<uint32_t>(std::round(height)), static_cast<img::Color>(background));

    for(const Line2D& line : lines)
    {
        image.draw_line(uint32_t(line.p1.x*scale + dx), uint32_t(line.p1.y*scale + dy), uint32_t(line.p2.x*scale + dx),uint32_t(line.p2.y*scale + dy), static_cast<img::Color>(line.color));
    }
    return image;
}



