//============================================================================
// @name        : index.cpp
// @author      : Thomas Dooms
// @date        : 5/10/19
// @version     : 
// @copyright   : BA1 Informatica - Thomas Dooms - University of Antwerp
// @description : 
//============================================================================

#include "index.h"

bool operator==(const Index& a, const Index& b)
{
    return a.p1 == b.p2 and a.p2 == b.p1;       // because every face is ccw we know the indices will be inversed.
}