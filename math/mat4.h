//============================================================================
// @name        : mat4.h
// @author      : Thomas Dooms
// @date        : 10/01/19
// @version     : 1.1
// @copyright   : BA1 Informatica - Thomas Dooms - University of Antwerp
// @description : math classes inspired by GLSL, uses ^ for elementwise multiplications.
//============================================================================

#pragma once

#include <iostream>
#include <vector>
#include "vec4.h"

struct mat4
{
    mat4() = default;
    mat4(const vec4& v0, const vec4& v1, const vec4& v2, const vec4& v3);
    mat4(const double* f);

    /*----------------------------------------*/

    static mat4 createUnitMatrix();
    static mat4 createTranslationMatrix(double x, double y, double z);
    static mat4 createScalarMatrix(double x, double y, double z);
    static mat4 createScalarMatrix(double scale);

    static mat4 createXRotationMatrix(double alpha);
    static mat4 createYRotationMatrix(double alpha);
    static mat4 createZRotationMatrix(double alpha);
    static mat4 createRotationMatrix(double alphaX, double alphaY, double alphaZ);

    static mat4 createTotalTranslationMatrix(const std::vector<double>& translation, double scale, const std::vector<double>& rotation);
    static mat4 createEyeTransformationMatrix(double x, double y, double z);

    /*----------------------------------------*/

    double& operator[](int index) { return data[index]; }
    double operator[](int index) const { return data[index]; }

    const double* begin() const { return data; }
    const double* end() const { return data+15; }

    /*----------------------------------------*/

    mat4& operator+=(const mat4& m);
    mat4& operator-=(const mat4& m);
    mat4& operator^=(const mat4& m);
    mat4& operator/=(const mat4& m);
    mat4& operator*=(const mat4& m);

    mat4& operator*=(double s);
    mat4& operator/=(double s);

    /*----------------------------------------*/

    friend mat4 operator+(const mat4& a, const mat4& b);
    friend mat4 operator-(const mat4& a, const mat4& b);
    friend mat4 operator^(const mat4& a, const mat4& b);
    friend mat4 operator/(const mat4& a, const mat4& b);
    friend mat4 operator*(const mat4& a, const mat4& b);

    friend mat4 operator-(const mat4& a);
    friend mat4 operator*(const mat4& a, double s);
    friend mat4 operator/(const mat4& a, double s);
    friend mat4 operator*(double s, const mat4& a);
    friend mat4 operator/(double s, const mat4& a);

    friend vec4 operator*(const mat4& a, const vec4& b);
    friend vec4 operator*(const vec4& a, const mat4& b);

    /*----------------------------------------*/

    friend std::ostream& operator<<(std::ostream& stream, const mat4& a);

    /*----------------------------------------*/

    double data[16];
};

