//============================================================================
// @name        : math.h
// @author      : Thomas Dooms
// @date        : 10/01/19
// @version     : 1.1
// @copyright   : BA1 Informatica - Thomas Dooms - University of Antwerp
// @description : math classes inspired by GLSL, uses ^ for elementwise multiplications.
//============================================================================

#pragma once

void matmul(const double* a, const double* b, double* res);
void matvecmul(const double* a, const double* b, double* res);
void vecmatmul(const double* a, const double* b, double* res);
