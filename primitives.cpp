#include "primitives.h"
#include "math.h"
#include <limits>
#include <algorithm>

img::EasyImage draw2DLines(const Lines2D& lines, const Color& background, const int size)
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

Figure3D parseFigure(const ini::Section& section)
{
    if(static_cast<std::string>(section["type"]) != "LineDrawing") std::cerr << "only linedrawing type is supported\n";

    Vec3 rotation    = {section["rotateX"], section["rotateY"], section["rotateZ"]};
    Vec3 translation = section["center"].as_double_tuple_or_die();
    Vec3 color       = section["color" ].as_double_tuple_or_die();

    double   scale       = section["scale"];
    uint32_t numVertices = static_cast<uint32_t>((int)section["nrPoints"]);
    uint32_t numIndices  = static_cast<uint32_t>((int)section["nrLines" ]);

    Mat4 transform = Mat4::createTotalTranslationMatrix(translation, scale, rotation * M_PI/180.0);

    Figure3D figure = {numVertices, numIndices, color};

    for(uint32_t i = 0; i < numVertices; i++)
    {
        const std::vector<double> point = section["point" + std::to_string(i)];
        figure.vertices[i] = {point[0], point[1], point[2]};
        figure.vertices[i] *= transform;
    }
    for(uint32_t i = 0; i < numIndices; i++)
    {
        const std::vector<int> point = section["line" + std::to_string(i)];
        figure.indices[i][0] = static_cast<uint32_t>(point[0]);
        figure.indices[i][1] = static_cast<uint32_t>(point[1]);
    }
    return figure;
}

Lines2D doProjection(Figures3D& figures, const Mat4& matrix, const double d)
{
    Lines2D lines;
    for(auto& figure : figures)
    {
        for (auto& point : figure.vertices)
        {
            point *= matrix;        // apply translation
            point *= -(d/point[2]);  // apply depth division
        }
        for (const auto& index : figure.indices)
        {
            const Vec2 p1 = figure.vertices[ index[0] ].xy();
            const Vec2 p2 = figure.vertices[ index[1] ].xy();
            lines.emplace_front(p1, p2, figure.color);
        }
    }
    return lines;
}



