//============================================================================
// @name        : zbuffer.h
// @author      : Thomas Dooms
// @date        : 3/12/19
// @version     : 
// @copyright   : BA1 Informatica - Thomas Dooms - University of Antwerp
// @description : 
//============================================================================


#ifndef ENGINE_ZBUFFER_H
#define ENGINE_ZBUFFER_H

#include <vector>
#include <stdint.h>


class ZBuffer
{
public:
    ZBuffer(double width, double height);
    bool operator()(uint32_t x, uint32_t y, double depth);

private:
    std::vector<double> buffer;

    uint32_t width;
    uint32_t height;
};


#endif //ENGINE_ZBUFFER_H
