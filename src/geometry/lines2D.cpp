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
#include <unordered_set>
#include <cmath>
#include <cassert>

struct Index
{
    Index(const uint32_t p1, const uint32_t p2) : p1(p1), p2(p2) {}
    friend bool operator==(const Index& a, const Index& b);

    uint32_t p1, p2;
};
bool operator==(const Index& a, const Index& b)
{
    return a.p1 == b.p2 and a.p2 == b.p1;       // because every face in ccw we know the indices will be inversed.
}

template<>
struct std::hash<Index>
{
    size_t operator()(const Index& a) const noexcept { return ((a.p1 << 16) + a.p2) + (((a.p2 << 16) + a.p1)); }
};


void drawLine(img::EasyImage& image, const Vec3& p1, const Vec3& p2, const Vec3& color)
{
    img::Color rgb = { static_cast<uint8_t>(color[0]*255.99), static_cast<uint8_t>(color[1]*255.99), static_cast<uint8_t>(color[2]*255.99) };
    auto x0 = static_cast<uint32_t>(std::round(p1[0]));
    auto y0 = static_cast<uint32_t>(std::round(p1[1]));
    auto x1 = static_cast<uint32_t>(std::round(p2[0]));
    auto y1 = static_cast<uint32_t>(std::round(p2[1]));

    assert(x0 < image.get_width() && y0 < image.get_height());
    assert(x1 < image.get_width() && y1 < image.get_height());

    if (x0 == x1)
    {
        for(uint32_t i = std::min(y0, y1); i <= std::max(y0, y1); i++) image(x0, i) = rgb;
    }
    else if (y0 == y1)
    {
        for(uint32_t i = std::min(x0, x1); i <= std::max(x0, x1); i++) image(i, y0) = rgb;
    }
    else
    {
        if (x0 > x1)
        {
            std::swap(x0, x1);
            std::swap(y0, y1);
        }
        double m = ((double) y1 - (double) y0) / ((double) x1 - (double) x0);
        if (-1.0 <= m && m <= 1.0)
        {
            for (uint32_t i = 0; i <= (x1 - x0); i++) image(x0 + i, (uint32_t) round(y0 + m * i)) = rgb;
        }
        else if (m > 1.0)
        {
            for (uint32_t i = 0; i <= (y1 - y0); i++) image((uint32_t) round(x0 + (i / m)), y0 + i) = rgb;
        }
        else if (m < -1.0)
        {
            for (uint32_t i = 0; i <= (y0 - y1); i++) image((uint32_t) round(x0 - (i / m)), y0 - i) = rgb;
        }
    }
}

void drawLine(img::EasyImage& image, ZBuffer& buffer, const Vec3& p1, const Vec3& p2, const Vec3& color)
{
    img::Color rgb = { static_cast<uint8_t>(color[0]*255.99), static_cast<uint8_t>(color[1]*255.99), static_cast<uint8_t>(color[2]*255.99) };
    auto x0 = static_cast<uint32_t>(std::round(p1[0]));
    auto y0 = static_cast<uint32_t>(std::round(p1[1]));
    auto x1 = static_cast<uint32_t>(std::round(p2[0]));
    auto y1 = static_cast<uint32_t>(std::round(p2[1]));
    auto z0 = p1[2];
    auto z1 = p2[2];

    assert(x0 < image.get_width() && y0 < image.get_height());
    assert(x1 < image.get_width() && y1 < image.get_height());

    const double pixels = std::max(std::max(x1, x0) - std::min(x1,x0), std::max(y1, y0) - std::min(y1,y0));
    const double step = (z1 - z0) / (z0 * z1 * pixels);     // simplification of (1 / z0 - 1/ z1) * (1/pixels)
    double depthIter = z0;

    if (x0 == x1)
    {
        for(uint32_t i = std::min(y0, y1); i <= std::max(y0, y1); i++)
        {
            if( buffer(x0, i, depthIter += step) ) image(x0, i) = rgb;
        }
    }
    else if (y0 == y1)
    {
        for(uint32_t i = std::min(x0, x1); i <= std::max(x0, x1); i++)
        {
            if( buffer(i, y0, depthIter += step) ) image(i, y0) = rgb;
        }
    }
    else
    {
        if (x0 > x1)
        {
            std::swap(x0, x1);
            std::swap(y0, y1);
        }
        double m = ((double) y1 - (double) y0) / ((double) x1 - (double) x0);
        if (-1.0 <= m && m <= 1.0)
        {
            for (uint32_t i = 0; i <= (x1 - x0); i++)
            {
                uint32_t x = x0 + i; uint32_t y = (uint32_t) round(y0 + m * i);
                if( buffer(x,y, depthIter += step) ) image(x, y) = rgb;
            }
        }
        else if (m > 1.0)
        {
            for (uint32_t i = 0; i <= (y1 - y0); i++)
            {
                uint32_t x = (uint32_t)round(x0 + (i / m)); uint32_t y = y0 + i;
                if( buffer(x,y, depthIter += step) ) image(x, y) = rgb;
            }
        }
        else if (m < -1.0)
        {
            for (uint32_t i = 0; i <= (y0 - y1); i++)
            {
                uint32_t x = (uint32_t) round(x0 - (i / m)); uint32_t y = y0 - i;
                if( buffer(x, y, depthIter += step) ) image(x,y) = rgb;
            }
        }
    }
}

img::EasyImage drawFigures(std::vector<Mesh>& figures, const Color& background, const uint32_t size, double d, bool depthBuffer)
{
    std::forward_list<Line2D> lines;

    for(int i = figures.size() - 1; i >= 0; i--)
    {
        for (auto& point : figures[i].vertices)
        {
            // apply depth division to x an y coordinates
            const double div = -(d/point[2]);
            point[0] *= div;
            point[1] *= div;
        }
        // size of the hashmap = amount of lines / 2 because half of the lines will be deleted
        std::unordered_set<Index> set(figures[i].indices.size() * figures[i].indices[1].size() / 2);
        for (const auto& face : figures[i].indices)
        {
            if(face.size() == 2) set.emplace(face[0], face[1]);
            else
            {
                for(uint32_t j = 0; j < face.size()-1; j++) set.emplace(face[j], face[j+1]);
                set.emplace(face[face.size() - 1], face[0]);
            }
        }
        for(const auto& index : set) lines.emplace_front(figures[i].vertices[index.p1], figures[i].vertices[index.p2], figures[i].color);
    }
    return drawLines(lines, background, size, depthBuffer);
}

img::EasyImage drawLines(const std::forward_list<Line2D>& lines, const Color& background, const int size, bool depthBuffer)
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

    if(depthBuffer)
    {
        ZBuffer buffer(width, height);
        for(const Line2D& line : lines)
        {
            drawLine(image, buffer, {line.p1[0]*scale + dx, line.p1[1]*scale + dy, line.p1[2]}, {line.p2[0]*scale + dx, line.p2[1]*scale + dy, line.p2[2]}, line.color);
        }
    }
    else
    {
        for(const Line2D& line : lines) drawLine(image, line.p1.xy()*scale + Vec2{dx, dy}, line.p2.xy()*scale + Vec2{dx, dy}, line.color);
    }
    return image;
}


