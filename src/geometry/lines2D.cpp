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
    return a.p1 == b.p2 and a.p2 == b.p1;       // because every face is ccw we know the indices will be inversed.
}

template<>
struct std::hash<Index>
{
    size_t operator()(const Index& a) const noexcept { return ((a.p1 << 16) + a.p2) + (((a.p2 << 16) + a.p1)); }
};


void drawLine(img::EasyImage& image, const Vec3& p1, const Vec3& p2, const Color& color)
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

void drawLine(img::EasyImage& image, ZBuffer& buffer, const Vec3& p1, const Vec3& p2, const Color& color)
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

    double step = (z1 - z0) / (std::max(std::abs(int(x1-x0)), std::abs(int(y1-y0))));
    double iter = z0;

    if (x0 == x1)
    {
        for(uint32_t i = std::min(y0, y1); i <= std::max(y0, y1); i++)
        {
            if( buffer(x0, i, iter)) image(x0, i) = rgb;
            iter += step;
        }
    }
    else if (y0 == y1)
    {
        for(uint32_t i = std::min(x0, x1); i <= std::max(x0, x1); i++)
        {
            if( buffer(i, y0, iter )) image(i, y0) = rgb;
            iter += step;
        }
    }
    else
    {
        if (x0 > x1)
        {
            std::swap(x0, x1);
            std::swap(y0, y1);
            std::swap(z0, z1);
            iter = z0;
        }
        double m = ((double) y1 - (double) y0) / ((double) x1 - (double) x0);
        if (-1.0 <= m && m <= 1.0)
        {
            for (uint32_t i = 0; i <= (x1 - x0); i++)
            {
                uint32_t x = x0 + i; uint32_t y = (uint32_t) round(y0 + m * i);
                if( buffer(x, y, iter) ) image(x, y) = rgb;
                iter -= step;
            }
        }
        else if (m > 1.0)
        {
            for (uint32_t i = 0; i <= (y1 - y0); i++)
            {
                uint32_t x = (uint32_t)round(x0 + (i / m)); uint32_t y = y0 + i;
                if( buffer(x, y, iter )) image(x, y) = rgb;
                iter -= step;
            }
        }
        else if (m < -1.0)
        {
            for (uint32_t i = 0; i <= (y0 - y1); i++)
            {
                uint32_t x = (uint32_t) round(x0 - (i / m)); uint32_t y = y0 - i;
                if( buffer(x, y, iter) ) image(x,y) = rgb;
                iter -= step;
            }
        }
    }
}

void drawTriangle(img::EasyImage& image, ZBuffer& buffer, const Vec3& p1, const Vec3& p2, const Vec3& p3, double d, const Vec2& dxy, const Color& color)
{
    img::Color rgb = { static_cast<uint8_t>(color[0]*255.99), static_cast<uint8_t>(color[1]*255.99), static_cast<uint8_t>(color[2]*255.99) };

    Vec3 min = p1;
    Vec3 mid = p2;
    Vec3 max = p3;

    min[0] *= -d/min[2]; min[1] *= -d/min[2];
    mid[0] *= -d/mid[2]; mid[1] *= -d/mid[2];
    max[0] *= -d/max[2]; max[1] *= -d/max[2];

    min += dxy;
    mid += dxy;
    max += dxy;

    if(min[1] > mid[1]) std::swap(min, mid);
    if(mid[1] > max[1]) std::swap(mid, max);
    if(min[1] > mid[1]) std::swap(min, mid);

    double stepMinMax = (max[0] - min[0])/(max[1] - min[1]);
    double stepMinMid = (mid[0] - min[0])/(mid[1] - min[1]);
    double stepMidMax = (max[0] - mid[0])/(max[1] - mid[1]);

    double minIter = min[0];
    double maxIter = min[0];

    Vec2 line1 = max.xy() - min.xy();
    Vec2 line2 = mid.xy() - min.xy();

    if(line1[0] * line2[1] - line1[1] * line2[0] <= 0)
    {
        for(auto y = uint32_t(std::round(min[1])); y < uint32_t(std::round(mid[1])); y++)
        {
            minIter += stepMinMax;
            maxIter += stepMinMid;
            drawXLine(image, y, minIter, maxIter, rgb);
        }
        for(auto y = uint32_t(std::round(mid[1])); y < uint32_t(std::round(max[1])); y++)
        {
            minIter += stepMinMax;
            maxIter += stepMidMax;
            drawXLine(image, y, minIter, maxIter, rgb);
        }
    }
    else
    {
        for(auto y = uint32_t(std::round(min[1])); y < uint32_t(std::round(mid[1])); y++)
        {
            minIter += stepMinMid;
            maxIter += stepMinMax;
            drawXLine(image, y, minIter, maxIter, rgb);
        }
        for(auto y = uint32_t(std::round(mid[1])); y < uint32_t(std::round(max[1])); y++)
        {
            minIter += stepMidMax;
            maxIter += stepMinMax;
            drawXLine(image, y, minIter, maxIter, rgb);
        }
    }
}

void drawXLine(img::EasyImage& image, uint32_t y, double xMin, double xMax, const img::Color& color)
{
    for(auto x = (uint32_t)std::round(xMin); x < (uint32_t)std::round(xMax); x++)
    {
        image(x,y) = color;
    }
}

img::EasyImage drawTriangulatedMeshes(const std::vector<Mesh>& figures, const Color& background, const uint32_t size)
{
    const auto ranges = getRanges(doProjection(figures, 1), size);
    const double d      = std::get<0>(ranges);
    const double width  = std::get<1>(ranges);
    const double height = std::get<2>(ranges);
    const double dx     = std::get<3>(ranges);
    const double dy     = std::get<4>(ranges);

    img::EasyImage image(width, height, background);
    ZBuffer buffer(width, height);

    for(const Mesh& figure : figures)
    {
        auto newIndices = Mesh::triangulate(figure.indices);
        for(const auto& triangle : newIndices)
        {
            drawTriangle(image, buffer, figure.vertices[triangle[0]], figure.vertices[triangle[1]], figure.vertices[triangle[2]], d, {dx, dy}, figure.color);
        }
    }
    return image;
}

std::forward_list<Line2D> doProjection(const std::vector<Mesh>& figures, const double d)
{
    std::forward_list<Line2D> lines;

    for(int i = figures.size() - 1; i >= 0; i--)
    {
        std::vector<Vec3> projectedVertices;
        projectedVertices.reserve(figures[i].vertices.size());
        for (const Vec3& point : figures[i].vertices)
        {
            // apply depth division to x an y coordinates
            Vec3 temp;
            const double div = -(d/point[2]);
            temp[0] = point[0]*div;
            temp[1] = point[1]*div;
            temp[2] = point[2];
            projectedVertices.push_back(temp);
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
        for(const auto& index : set) lines.emplace_front(projectedVertices[index.p1], projectedVertices[index.p2], figures[i].color);
    }
    return lines;
}


img::EasyImage drawFigures(const std::vector<Mesh>& figures, const Color& background, const uint32_t size, const double d, const bool depthBuffer)
{
    auto lines = doProjection(figures, d);
    return drawLines(lines, background, size, depthBuffer);
}


std::tuple<double, double, double, double, double> getRanges(const std::forward_list<Line2D>& lines, const double size)
{
    const auto x0 = std::minmax_element(begin(lines), end(lines), [](const Line2D& a, const Line2D& b){ return a.p1[0] < b.p1[0]; });
    const auto x1 = std::minmax_element(begin(lines), end(lines), [](const Line2D& a, const Line2D& b){ return a.p2[0] < b.p2[0]; });
    const auto y0 = std::minmax_element(begin(lines), end(lines), [](const Line2D& a, const Line2D& b){ return a.p1[1] < b.p1[1]; });
    const auto y1 = std::minmax_element(begin(lines), end(lines), [](const Line2D& a, const Line2D& b){ return a.p2[1] < b.p2[1]; });

    const double xMax = std::max((*x0.second).p1[0], (*x1.second).p2[0]);
    const double xMin = std::min((*x0.first ).p1[0], (*x1.first ).p2[0]);
    const double yMax = std::max((*y0.second).p1[1], (*y1.second).p2[1]);
    const double yMin = std::min((*y0.first ).p1[1], (*y1.first ).p2[1]);

    const double xRange = xMax - xMin;
    const double yRange = yMax - yMin;

    const double width  = size * xRange / std::max(xRange, yRange);
    const double height = size * yRange / std::max(xRange, yRange);
    const double scale  = 0.95 * width  / xRange;

    const double dx = (width  - scale * (xMax + xMin)) / 2.0;
    const double dy = (height - scale * (yMax + yMin)) / 2.0;

    return {scale, width, height, dx, dy};
}

img::EasyImage drawLines(const std::forward_list<Line2D>& lines, const Color& background, const int size, const bool depthBuffer)
{
    const auto ranges   = getRanges(lines, size);
    const double scale  = std::get<0>(ranges);
    const double width  = std::get<1>(ranges);
    const double height = std::get<2>(ranges);
    const double dx     = std::get<3>(ranges);
    const double dy     = std::get<4>(ranges);

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


