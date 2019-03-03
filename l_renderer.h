/*
 * @summary: A class to encapsulate all logic to rendering from an l-system
 * @author: Thomas Dooms
 * @date: 22/02/2019
 * @version: 1.0
 */

#ifndef ENGINE_L_RENDERER_H
#define ENGINE_L_RENDERER_H

#include "l_parser.h"
#include "primitives.h"
#include "easy_image.h"
#include <stack>
#include <cmath>

class LSystemRenderer
{
public:
    LSystemRenderer(const LParser::LSystem2D& l_system, Color lineColor);

    img::EasyImage generateImage(const Color& background, int size) const;

private:
    inline bool exists(char c) const;
    inline void recursiveEval(char symbol, uint32_t depth);
    inline void addLine();

    std::stack<std::pair<vec2, double>> pos;
    double angle;
    uint32_t maxDepth;

    Lines2D lines;
    Color lineColor;

    const LParser::LSystem2D& info;
};


#endif //ENGINE_L_RENDERER_H
