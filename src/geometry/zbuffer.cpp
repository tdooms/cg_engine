//============================================================================
// @name        : zbuffer.cpp
// @author      : Thomas Dooms
// @date        : 3/12/19
// @version     : 
// @copyright   : BA1 Informatica - Thomas Dooms - University of Antwerp
// @description : 
//============================================================================

#include "zbuffer.h"
#include "../easy_image.h"

#include <cmath>
#include <cassert>
#include <limits>
#include <iostream>
#include <fstream>


ZBuffer::ZBuffer(double _width, double _height)
{
    width  = static_cast<uint32_t>(std::round(_width ));
    height = static_cast<uint32_t>(std::round(_height));

    double value = std::numeric_limits<double>::max();
    buffer = std::vector<double>(width*height, value);
}

double ZBuffer::operator()(uint32_t x, uint32_t y) const
{
    return buffer[x*height+y];
}

bool ZBuffer::operator()(uint32_t x, uint32_t y, double depth)
{
    const uint32_t index = x * height + y;
    if(depth <= buffer[index])
    {
        buffer[index] = depth;
        return true;
    }
    return false;
}

void ZBuffer::toImage() const
{
    img::EasyImage image(width, height, {});

    for(uint32_t i = 0; i < width*height; i++) image(i) = {static_cast<uint8_t>(-5 / buffer[i]), static_cast<uint8_t>(-5 / buffer[i]), static_cast<uint8_t>(-5 / buffer[i])};

    try
    {
        std::ofstream f_out("depthbuffer.bmp", std::ios::trunc | std::ios::out | std::ios::binary);
        f_out << image;

    }
    catch(std::exception& ex)
    {
        std::cerr << "Failed to write image to file: " << ex.what() << std::endl;
        exit(1);
    }
}