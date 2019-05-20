//============================================================================
// @name        : math.cpp
// @author      : Thomas Dooms
// @date        : 10/01/19
// @version     : 1.1
// @copyright   : BA1 Informatica - Thomas Dooms - University of Antwerp
// @description : math classes inspired by GLSL, uses ^ for elementwise multiplications.
//============================================================================

#include "math.h"
#include <array>


void matmul(const double* const a, const double* const b, double* const res)
{
    res[0]  = a[0] *b[0] + a[1] *b[4] + a[2] *b[8]  + a[3] *b[12];
    res[1]  = a[0] *b[1] + a[1] *b[5] + a[2] *b[9]  + a[3] *b[13];
    res[2]  = a[0] *b[2] + a[1] *b[6] + a[2] *b[10] + a[3] *b[14];
    res[3]  = a[0] *b[3] + a[1] *b[7] + a[2] *b[11] + a[3] *b[15];

    res[4]  = a[4] *b[0] + a[5] *b[4] + a[6] *b[8]  + a[7] *b[12];
    res[5]  = a[4] *b[1] + a[5] *b[5] + a[6] *b[9]  + a[7] *b[13];
    res[6]  = a[4] *b[2] + a[5] *b[6] + a[6] *b[10] + a[7] *b[14];
    res[7]  = a[4] *b[3] + a[5] *b[7] + a[6] *b[11] + a[7] *b[15];

    res[8]  = a[8] *b[0] + a[9] *b[4] + a[10]*b[8]  + a[11]*b[12];
    res[9]  = a[8] *b[1] + a[9] *b[5] + a[10]*b[9]  + a[11]*b[13];
    res[10] = a[8] *b[2] + a[9] *b[6] + a[10]*b[10] + a[11]*b[14];
    res[11] = a[8] *b[3] + a[9] *b[7] + a[10]*b[11] + a[11]*b[15];

    res[12] = a[12]*b[0] + a[13]*b[4] + a[14]*b[8]  + a[15]*b[12];
    res[13] = a[12]*b[1] + a[13]*b[5] + a[14]*b[9]  + a[15]*b[13];
    res[14] = a[12]*b[2] + a[13]*b[6] + a[14]*b[10] + a[15]*b[14];
    res[15] = a[12]*b[3] + a[13]*b[7] + a[14]*b[11] + a[15]*b[15];
}

void matvecmul(const double* const a, const double* const b, double* const res)
{
    res[0] = a[0] *b[0] + a[1] *b[1] + a[2] *b[2] + a[3] *b[3];
    res[1] = a[4] *b[0] + a[5] *b[1] + a[6] *b[2] + a[7] *b[3];
    res[2] = a[8] *b[0] + a[9] *b[1] + a[10]*b[2] + a[11]*b[3];
    res[3] = a[12]*b[0] + a[13]*b[1] + a[14]*b[2] + a[15]*b[3];
}

void vecmatmul(const double* const a, const double* const b, double* const res)
{
    res[0] = a[0] *b[0] + a[1] *b[4] + a[2] *b[8] + a[3]*b[12];
    res[1] = a[0] *b[1] + a[1] *b[5] + a[2] *b[9] + a[3]*b[13];
    res[2] = a[0] *b[2] + a[1] *b[6] + a[2]*b[10] + a[3]*b[14];
    res[3] = a[0] *b[3] + a[1] *b[7] + a[2]*b[11] + a[3]*b[15];
}

double det3x3(const double* a, uint8_t i, uint8_t j)
{
    uint32_t iter = 0;
    std::array<uint8_t, 9> indices;
    for(uint32_t k = 0; k < 16; k++)
    {
        if(k % 4 == j or k / 4 == i) continue;
        indices[iter] = k;
        iter++;
    }
    return a[indices[0]] * ( a[indices[4]]*a[indices[8]] - a[indices[7]]*a[indices[5]]) -
            a[indices[1]] * ( a[indices[3]]*a[indices[8]] - a[indices[6]]*a[indices[5]]) +
            a[indices[2]] * ( a[indices[3]]*a[indices[7]] - a[indices[6]]*a[indices[4]]);
}