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

Figure3D parseFigure(const ini::Section& section)
{
    if(static_cast<std::string>(section["type"]) != "LineDrawing") std::cerr << "only linedrawing type is supported\n";

    std::vector<double> rotation = {section["rotateX"], section["rotateY"], section["rotateZ"]};
    for(auto& item : rotation) item *= M_PI / 180.0;
    mat4 transform = mat4::createTotalTranslationMatrix(section["center"], section["scale"], rotation);

    uint32_t numPoints = static_cast<uint32_t>((int)section["nrPoints"]);
    uint32_t numLines  = static_cast<uint32_t>((int)section["nrLines" ]);

    Figure3D figure;
    figure.points = std::vector<vec4>();
    figure.faces = std::vector<std::vector<uint32_t>>();
    figure.points.reserve(numPoints);
    figure.faces.reserve(numLines);
    figure.color = {section["color"]};

    for(uint32_t i = 0; i < numPoints; i++)
    {
        const std::vector<double> point = section["point" + std::to_string(i)];
        figure.points.emplace_back(point[0], point[1], point[2]);
        figure.points[i] *= transform;
    }
    for(uint32_t i = 0; i < numLines; i++)
    {
        const std::vector<int> point = section["line" + std::to_string(i)];
        figure.faces.push_back({(uint32_t)point[0], (uint32_t)point[1]});
    }
    return figure;
}

Lines2D doProjection(Figures3D& figures, const mat4& matrix, const double d)
{
    Lines2D lines;
    for(auto& figure : figures)
    {
        for (auto& point : figure.points)
        {
            point *= matrix;        // apply translation
            point *= -(d/point[2]);  // apply depth division
        }
        for (const auto& face : figure.faces)
        {
            const Point2D p1 = {figure.points[face[0]].getX(), figure.points[face[0]].getY()};
            const Point2D p2 = {figure.points[face[1]].getX(), figure.points[face[1]].getY()};
            lines.emplace_front(p1, p2, figure.color);
        }
    }
    return lines;
}



