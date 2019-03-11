#include "l_renderer.h"


LSystem2DRenderer::LSystem2DRenderer(const LParser::LSystem2D& info, const Color& lineColor) : lineColor(lineColor), info(info)
{
    maxDepth = info.get_nr_iterations();
    angle    = info.get_angle() * M_PI/180.0;

    pos.push({{0,0}, info.get_starting_angle() * M_PI/180.0});

    for(char symbol : info.get_initiator()) recursiveEval(symbol, 0);
}

img::EasyImage LSystem2DRenderer::generateImage(const Color& background, int size) const
{
    return drawLines(lines, background, size);
}

void LSystem2DRenderer::recursiveEval(char symbol, uint32_t depth)
{
    if      (symbol == '-') pos.top().second -= angle;
    else if (symbol == '+') pos.top().second += angle;
    else if (symbol == '(') pos.push(pos.top());
    else if (symbol == ')') pos.pop();
    else if (depth == maxDepth && exists(symbol)) addLine();
    else if (exists(symbol)) for(char c : info.get_replacement(symbol)) recursiveEval(c, depth+1);
    else std::cerr << "unknown symbol\n";
}

bool LSystem2DRenderer::exists(char c) const
{
    return info.get_alphabet().find(c) != end(info.get_alphabet());
}

void LSystem2DRenderer::addLine()
{
    double newX = pos.top().first[0] + std::cos(pos.top().second);
    double newY = pos.top().first[1] + std::sin(pos.top().second);
    lines.emplace_front(pos.top().first, Vec2{newX, newY}, lineColor);
    pos.top().first = {newX, newY};
}


LSystem3DRenderer::LSystem3DRenderer(const LParser::LSystem3D& info, const Color& lineColor) : lineColor(lineColor), info(info)
{
    maxDepth = info.get_nr_iterations();
    angle    = info.get_angle() * M_PI/180.0;

    lastIndex = 0;

    pos.push({{0,0,0}, {1,0,0}, {0,1,0}, {0,0,1}, 0});

    for(char symbol : info.get_initiator()) recursiveEval(symbol, 0);
}

Mesh LSystem3DRenderer::generateMesh() const
{
    std::vector<Vec3> vertices(lastIndex+1);
    std::vector<std::vector<uint32_t>> indices(lastIndex);


    uint32_t iter = lastIndex;
    for(const auto& line : lines)
    {
        vertices[iter] = std::get<0>(line);
        indices[iter-1] = {std::get<1>(line), std::get<2>(line)};
        iter--;
    }
    vertices[0] = {0,0,0};
    return {vertices, indices, lineColor};
}

void LSystem3DRenderer::recursiveEval(char symbol, uint32_t depth)
{
    if      (symbol == '(') pos.push(pos.top());
    else if (symbol == ')') pos.pop();
    else if (symbol == '+')
    {
        const Vec3 temp = pos.top().h;
        pos.top().h = temp * cos(angle) + pos.top().l * sin(angle);
        pos.top().l = -temp * sin(angle) + pos.top().l * cos(angle);
    }
    else if (symbol == '-')
    {
        const Vec3 temp = pos.top().h;
        pos.top().h = pos.top().h * cos(-angle) + pos.top().l * sin(-angle);
        pos.top().l = -temp * sin(-angle) + pos.top().l * cos(-angle);
    }
    else if(symbol == '^')
    {
        const Vec3 temp = pos.top().h;
        pos.top().h =  temp * cos(angle) + pos.top().u * sin(angle);
        pos.top().u = -temp * sin(angle) + pos.top().u * cos(angle);
    }
    else if (symbol == '&')
    {
        const Vec3 temp = pos.top().h;
        pos.top().h =  temp * cos(-angle) + pos.top().u * sin(-angle);
        pos.top().u = -temp * sin(-angle) + pos.top().u * cos(-angle);
    }
    else if(symbol == '\\')
    {
        const Vec3 temp = pos.top().l;
        pos.top().l = temp * cos(angle) - pos.top().u * sin(angle);
        pos.top().u = temp * sin(angle) + pos.top().u * cos(angle);
    }
    else if (symbol == '/')
    {
        const Vec3 temp = pos.top().l;
        pos.top().l = temp * cos(-angle) - pos.top().u * sin(-angle);
        pos.top().u = temp * sin(-angle) + pos.top().u * cos(-angle);
    }
    else if(symbol == '|')
    {
        pos.top().h = -pos.top().h;
        pos.top().l = -pos.top().l;
    }
    else if (depth == maxDepth && exists(symbol)) addLine();
    else if (exists(symbol)) for(char c : info.get_replacement(symbol)) recursiveEval(c, depth+1);
    else std::cerr << "unknown symbol\n";
}

bool LSystem3DRenderer::exists(char c) const
{
    return info.get_alphabet().find(c) != end(info.get_alphabet());
}

void LSystem3DRenderer::addLine()
{
    pos.top().p += pos.top().h;                                     // P += H
    lines.emplace_front(pos.top().p, pos.top().index, ++lastIndex); // add new line
    pos.top().index = lastIndex;                                    // rewrite the last used index into top
}