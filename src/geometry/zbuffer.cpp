//============================================================================
// @name        : zbuffer.cpp
// @author      : Thomas Dooms
// @date        : 3/12/19
// @version     : 
// @copyright   : BA1 Informatica - Thomas Dooms - University of Antwerp
// @description : 
//============================================================================

#include "zbuffer.h"

#include <cmath>
#include <cassert>
#include <limits>


ZBuffer::ZBuffer(double _width, double _height)
{
    width  = static_cast<uint32_t>(std::round(_width ));
    height = static_cast<uint32_t>(std::round(_height));

    double value = -std::numeric_limits<double>::max();
    buffer = std::vector<double>(width*height, value);
}

bool ZBuffer::operator()(uint32_t x, uint32_t y, double depth)
{
    const uint32_t index = x * height + y;
    if(depth > buffer[index])
    {
        buffer[index] = depth;
        return true;
    }
    return false;
}