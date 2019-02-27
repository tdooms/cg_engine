#include "l_renderer.h"


LSystemRenderer::LSystemRenderer(const LParser::LSystem2D& info, Color lineColor) : lineColor(lineColor), info(info)
{
    maxDepth = info.get_nr_iterations();
    angle    = info.get_angle() * M_PI/180.0;

    pos.push({{0,0}, info.get_starting_angle() * M_PI/180.0});

    for(char symbol : info.get_initiator()) recursiveEval(symbol, 0);
}

img::EasyImage LSystemRenderer::generateImage(const Color& background, int size) const
{
    return draw2DLines(lines, background, size);
}

void LSystemRenderer::recursiveEval(char symbol, uint32_t depth)
{
    if      (symbol == '-') pos.top().second -= angle;
    else if (symbol == '+') pos.top().second += angle;
    else if (symbol == '(') pos.push(pos.top());
    else if (symbol == ')') pos.pop();
    else if (depth == maxDepth && exists(symbol)) addLine();
    else if (exists(symbol)) for(char c : info.get_replacement(symbol)) recursiveEval(c, depth+1);
    else std::cerr << "unknown symbol\n";
}

bool LSystemRenderer::exists(char c) const
{
    return info.get_alphabet().find(c) != end(info.get_alphabet());
}

void LSystemRenderer::addLine()
{
    double newX = pos.top().first.x + std::cos(pos.top().second);
    double newY = pos.top().first.y + std::sin(pos.top().second);
    lines.emplace_front(pos.top().first, Point2D{newX, newY}, lineColor);
    pos.top().first = {newX, newY};
}