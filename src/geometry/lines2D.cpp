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
#include "index.h"


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

    double step;
    double iter;

    if (x0 == x1)
    {
        if(y0 > y1)
        {
            std::swap(z0, z1);
            std::swap(y0, y1);
        }
        iter = z0;
        step = (z1-z0)/(y1- y0);

        for(uint32_t i = y0; i <= y1; i++)
        {
            if( buffer(x0, i, 1.0/iter)) image(x0, i) = rgb;
            iter += step;
        }
    }
    else if (y0 == y1)
    {
        if(x0 > x1)
        {
            std::swap(z0, z1);
            std::swap(x0, x1);
        }
        iter = z0;
        step = (z1-z0)/(x1 - x0);

        for(uint32_t i = x0; i <= x1; i++)
        {
            if( buffer(i, y0, 1.0/iter )) image(i, y0) = rgb;
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
        }
        double m = ((double) y1 - (double) y0) / ((double) x1 - (double) x0);
        if (-1.0 <= m && m <= 1.0)
        {
            iter = z0;
            step = (z1-z0)/(x1-x0);

            for (uint32_t i = 0; i <= (x1 - x0); i++)
            {
                uint32_t x = x0 + i; uint32_t y = (uint32_t) round(y0 + m * i);
                if( buffer(x, y, 1.0/iter) ) image(x, y) = rgb;
                iter += step;
            }
        }
        else if (m > 1.0)
        {
            iter = z0;
            step = (z1-z0)/(y1-y0);

            for (uint32_t i = 0; i <= (y1 - y0); i++)
            {
                uint32_t x = (uint32_t)round(x0 + (i / m)); uint32_t y = y0 + i;
                if( buffer(x, y, 1.0/iter )) image(x, y) = rgb;
                iter += step;
            }
        }
        else if (m < -1.0)
        {
            iter = z0;
            step = (z1-z0)/(y0-y1);

            for (uint32_t i = 0; i <= (y0 - y1); i++)
            {
                uint32_t x = (uint32_t) round(x0 - (i / m)); uint32_t y = y0 - i;
                if( buffer(x, y, 1.0/iter) ) image(x,y) = rgb;
                iter += step;
            }
        }
    }
}

void calculateLights(std::array<Color, 3>& colors, const Vec3& pos, const Vec3& normal, double reflection, bool directional, const Light& light)
{
    const Vec3 l = (directional) ? -light.vector : normalize(light.vector - pos);

    colors[0] += light.light[0];

    const double cosa = dot(l, normal);
    if(cosa > 0) colors[1] += light.light[1] * cosa;

    const Vec3 r = 2*cosa*normal - l;
    const double cosb = dot(-normalize(pos), normalize(r));
    if(cosb > 0) colors[2] += (light.light[2] * std::pow(cosb, reflection));
}

img::Color combineLights(std::array<Color, 3>& colors, const Mesh::Material& material, const Vec3* pos, const Texture* texture)
{
    if(texture == nullptr)
    {
        colors[0] ^= material.ambient;
        colors[1] ^= material.diffuse;
        colors[2] ^= material.specular;
    }
    else
    {
        const auto& material = Texture::getColor(texture, *pos);
        colors[0] ^= material.ambient;
        colors[1] ^= material.diffuse;
        colors[2] ^= material.specular;
    }

    Color color = colors[0] + colors[1] + colors[2];
    color[0] = std::min(color[0], 1.0); color[1] = std::min(color[1], 1.0); color[2] = std::min(color[2], 1.0);
    return { static_cast<uint8_t>(color[0]*255.99), static_cast<uint8_t>(color[1]*255.99), static_cast<uint8_t>(color[2]*255.99) };
}

void drawTriangle(img::EasyImage& image, ZBuffer& buffer, const Vec3& p1, const Vec3& p2, const Vec3& p3, double d, const Vec2& dxy,
        const Mesh::Material& material, const std::vector<Light>& directional, const std::vector<Light>& point, bool depthOnly, const Mat4& invEye, const Texture* texture)
{
    Vec3 temp1 = p1;
    Vec3 temp2 = p2;
    Vec3 temp3 = p3;

    temp1[0] *= -d/temp1[2]; temp1[1] *= -d/temp1[2];
    temp2[0] *= -d/temp2[2]; temp2[1] *= -d/temp2[2];
    temp3[0] *= -d/temp3[2]; temp3[1] *= -d/temp3[2];

    if(!depthOnly and ( ((p2[0] - p1[0]) * (p3[1] - p1[1]) - (p3[0] - p1[0]) * (p2[1] - p1[1])) < 0 )) return; // backface culling

    temp1 += dxy;
    temp2 += dxy;
    temp3 += dxy;

    const auto yMax = static_cast<uint32_t>(std::ceil(std::max(std::max(temp1[1], temp2[1]), temp3[1])));
    const auto yMin = static_cast<uint32_t>(std::ceil(std::min(std::min(temp1[1], temp2[1]), temp3[1])));

    const Vec3 g = (temp1 + temp2 + temp3) / 3.0;
    const Vec3 w = cross(p2 - p1, p3 - p1);
    const Vec3 normal = normalize(w);

    const double middleZ = 1.0/(3.0*p1[2]) + 1.0/(3.0*p2[2]) + 1.0/(3.0*p3[2]);
    const double k = dot(p1, w);
    const double dzdx = w[0] / (-d * k);
    const double dzdy = w[1] / (-d * k);

    const double step21 = (temp2[0] - temp1[0]) / (temp2[1] - temp1[1]);
    const double step32 = (temp3[0] - temp2[0]) / (temp3[1] - temp2[1]);
    const double step31 = (temp3[0] - temp1[0]) / (temp3[1] - temp1[1]);

    std::array<Color, 3> colors;  // ambient, diffuse, specular
    img::Color rgb;

    for(const auto& light : directional) calculateLights(colors, (p1+p2+p3)/3, normal, material.reflection, true, light);
    if(point.empty() and texture == nullptr)
    {
        rgb = combineLights(colors, material);
    }

    double xMin;
    double xMax;
    double xVal;

    for(uint32_t y = yMin; y < yMax; y++)
    {
        xMin =  std::numeric_limits<double>::max();
        xMax = -std::numeric_limits<double>::max();

        if(temp1[1] != temp2[1] and (y - temp2[1])*(y - temp1[1]) <= 0)
        {
            xVal = temp1[0] + step21 * (y - temp1[1]);
            xMin = std::min(xVal, xMin);
            xMax = std::max(xVal, xMax);
        }
        if(temp2[1] != temp3[1] and (y - temp3[1])*(y - temp2[1]) <= 0)
        {
            xVal = temp2[0] + step32 * (y - temp2[1]);
            xMin = std::min(xVal, xMin);
            xMax = std::max(xVal, xMax);
        }
        if(temp1[1] != temp3[1] and (y - temp3[1])*(y - temp1[1]) <= 0)
        {
            xVal = temp1[0] + step31 * (y - temp1[1]);
            xMin = std::min(xVal, xMin);
            xMax = std::max(xVal, xMax);
        }
        for(auto x = (uint32_t)std::ceil(xMin); x < (uint32_t)std::ceil(xMax); x++)
        {
            const double depth = middleZ + (x - g[0])*dzdx + (y - g[1])*dzdy;

            if(!point.empty())
            {
                const Vec2 tempVec = {double(x), double(y)};
                const Vec3 pos = { (tempVec-dxy)/(-depth*d), 1.0/depth };
                Vec3 real = pos;
                real *= invEye;

                std::array<Color, 3> pixelColors = colors; // ambient, diffuse, specular
                for(const auto& light : point)
                {
                    if(light.shadow)
                    {
                        Vec3 temp = real;
                        temp *= light.eye;
                        const double inv = -light.dValues[0]/temp[2];
                        temp = {temp[0]*inv + light.dValues[1], temp[1]*inv + light.dValues[2], 1.0/temp[2]};

                        const double xMantissa = temp[0] - std::floor(temp[0]);
                        const double yMantissa = temp[1] - std::floor(temp[1]);

                        const double inter1 = (1-xMantissa) * light.shadowMask(std::floor(temp[0]), std::floor(temp[1])) + xMantissa * light.shadowMask(std::ceil(temp[0]), std::floor(temp[1]));
                        const double inter2 = (1-xMantissa) * light.shadowMask(std::floor(temp[0]), std::ceil( temp[1])) + xMantissa * light.shadowMask(std::ceil(temp[0]), std::ceil( temp[1]));
                        const double realDepth = (1-yMantissa)*inter1 + yMantissa*inter2;

                        if(std::abs(temp[2] - realDepth) < 0.0001) calculateLights(pixelColors, pos, normal, material.reflection, false, light);
                        else pixelColors[0] += light.light[0];  // ambient
                    }
                    else calculateLights(pixelColors, pos, normal, material.reflection, false, light);
                }
                rgb = combineLights(pixelColors, material, &pos, texture);
            }
            if( buffer(x, y, depth) and !depthOnly ) image(x,y) = rgb;
        }
    }
}

std::tuple<img::EasyImage, ZBuffer, std::tuple<double, double, double, double, double>> drawTriangulatedMeshes(const std::vector<Mesh>& figures, const Color& background,
        uint32_t size, const std::vector<Light>& directional, const std::vector<Light>& point, const Mat4& eye, bool depthOnly, const Texture* texture)
{
    std::vector<Mesh> newFigures = figures;
    for(auto& figure : newFigures) figure *= eye;

    const auto ranges   = getRanges(newFigures, size, 1);
    const double d      = std::get<0>(ranges);
    const double width  = std::get<1>(ranges);
    const double height = std::get<2>(ranges);
    const double dx     = std::get<3>(ranges);
    const double dy     = std::get<4>(ranges);

    const Mat4 invEye = Mat4::inverse(eye);

    img::EasyImage image;
    if(!depthOnly) image = {width, height, background};

    ZBuffer buffer(width, height);

    for(const Mesh& figure : newFigures)
    {
        auto newIndices = Mesh::triangulate(figure.indices);
        for(const auto& triangle : newIndices)
        {
            drawTriangle(image, buffer, figure.vertices[triangle[0]],  figure.vertices[triangle[1]],  figure.vertices[triangle[2]], d, {dx, dy}, figure.material, directional, point, depthOnly, invEye, texture);
        }
    }
    return {image, buffer, ranges};
}

std::forward_list<Line2D> doProjection(const std::vector<Mesh>& figures, const double d, const Mat4& eye)
{
    std::forward_list<Line2D> lines;

    for(int i = figures.size() - 1; i >= 0; i--)
    {
        std::vector<Vec3> projectedVertices;
        projectedVertices.reserve(figures[i].vertices.size());
        for (const Vec3& point : figures[i].vertices)
        {
            // apply depth division to x an y coordinates
            Vec3 temp = point;
            temp *= eye;
            const double div = -(d/temp[2]);
            temp[0] = temp[0]*div;
            temp[1] = temp[1]*div;
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
        for(const auto& index : set) lines.emplace_front(projectedVertices[index.p1], projectedVertices[index.p2], figures[i].material.ambient);
    }
    return lines;
}


img::EasyImage drawFigures(const std::vector<Mesh>& figures, const Color& background, const uint32_t size, const double d, const bool depthBuffer, const Mat4& eye)
{
    auto lines = doProjection(figures, d, eye);
    return drawLines(lines, background, size, depthBuffer);
}

std::tuple<double, double, double, double, double> getRanges(const std::vector<Mesh>& meshes, double size, double d)
{
    double xMax = -std::numeric_limits<double>::max();
    double xMin =  std::numeric_limits<double>::max();
    double yMax = -std::numeric_limits<double>::max();
    double yMin =  std::numeric_limits<double>::max();

    for(const Mesh& mesh : meshes)
    {
        for(const Vec3& vertex : mesh.vertices)
        {
            const double div = -(d/vertex[2]);
            Vec2 proj = {vertex[0] * div, vertex[1] * div};
            xMax = std::max(proj[0], xMax);
            xMin = std::min(proj[0], xMin);
            yMax = std::max(proj[1], yMax);
            yMin = std::min(proj[1], yMin);
        }
    }

    const double xRange = xMax - xMin;
    const double yRange = yMax - yMin;

    const double width  = size * xRange / std::max(xRange, yRange);
    const double height = size * yRange / std::max(xRange, yRange);
    const double scale  = 0.95 * width  / xRange;

    const double dx = (width  - scale * (xMax + xMin)) / 2.0;
    const double dy = (height - scale * (yMax + yMin)) / 2.0;

    return {scale, width, height, dx, dy};
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
        for(const Line2D& line : lines)
            drawLine(image, line.p1.xy()*scale + Vec2{dx, dy}, line.p2.xy()*scale + Vec2{dx, dy}, line.color);
    }
    return image;
}


