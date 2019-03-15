/*
 * @summary: A class to encapsulate all logic to rendering from an l-system
 * @author: Thomas Dooms
 * @date: 22/02/2019
 * @version: 1.0
 */

#ifndef ENGINE_L_RENDERER_H
#define ENGINE_L_RENDERER_H

#include "l_parser.h"
#include "geometry/lines2D.h"
#include "easy_image.h"
#include <stack>
#include <cmath>

class LSystem2DRenderer
{
public:
    LSystem2DRenderer(const LParser::LSystem2D& l_system, const Color& lineColor);
    img::EasyImage generateImage(const Color& background, int size) const;

private:
    inline bool exists(char c) const;
    inline void recursiveEval(char symbol, uint32_t depth);
    inline void addLine();

    std::stack<std::pair<Vec2, double>> pos;
    double angle;
    uint32_t maxDepth;

    std::forward_list<Line2D> lines;
    Color lineColor;

    bool hasChanged = true;

    const LParser::LSystem2D& info;
};


struct Pos
{
    Vec3 p;
    Vec3 h;
    Vec3 l;
    Vec3 u;
    uint32_t index;
};


class LSystem3DRenderer
{
public:
    LSystem3DRenderer(const LParser::LSystem3D& l_system, const Color& lineColor);
    Mesh generateMesh() const;

private:
    inline bool exists(char c) const;
    inline void recursiveEval(char symbol, uint32_t depth);
    inline void addLine();

    // Position - Forward(H) - Left(L) - Up(U) - last index
    std::stack<Pos> pos;
    double angle;
    uint32_t maxDepth;

    std::forward_list<std::tuple<Vec3, uint32_t, uint32_t>> lines;

    uint32_t lastIndex;
    Color lineColor;

    const LParser::LSystem3D& info;
};


#endif //ENGINE_L_RENDERER_H
