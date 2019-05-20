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

struct Mat4
{
    Mat4() = default;
    Mat4(const Vec4& v0, const Vec4& v1, const Vec4& v2, const Vec4& v3);
    Mat4(const double* f);

    /*----------------------------------------*/

    static Mat4 createUnitMatrix();
    static Mat4 createTranslationMatrix(double x, double y, double z);
    static Mat4 createTranslationMatrix(const Vec3& xyz);
    static Mat4 createScalarMatrix(double x, double y, double z);
    static Mat4 createScalarMatrix(double scale);

    static Mat4 createXRotationMatrix(double alpha);
    static Mat4 createYRotationMatrix(double alpha);
    static Mat4 createZRotationMatrix(double alpha);
    static Mat4 createRotationMatrix(double alphaX, double alphaY, double alphaZ);

    static Mat4 createTotalTranslationMatrix(const Vec3& translation, double scale, const Vec3& rotation);
    static Mat4 createEyeTransformationMatrix(double x, double y, double z);

    /*----------------------------------------*/

    static double determinant(const Mat4& m);
    static Mat4 adjugate(const Mat4& m);
    static Mat4 inverse(const Mat4& m);

    /*----------------------------------------*/

    double& operator[](int index) { return data[index]; }
    double operator[](int index) const { return data[index]; }

    const double* begin() const { return data; }
    const double* end() const { return data+15; }

    /*----------------------------------------*/

    Mat4& operator+=(const Mat4& m);
    Mat4& operator-=(const Mat4& m);
    Mat4& operator^=(const Mat4& m);
    Mat4& operator/=(const Mat4& m);
    Mat4& operator*=(const Mat4& m);

    Mat4& operator*=(double s);
    Mat4& operator/=(double s);

    /*----------------------------------------*/

    friend Mat4 operator+(const Mat4& a, const Mat4& b);
    friend Mat4 operator-(const Mat4& a, const Mat4& b);
    friend Mat4 operator^(const Mat4& a, const Mat4& b);
    friend Mat4 operator/(const Mat4& a, const Mat4& b);
    friend Mat4 operator*(const Mat4& a, const Mat4& b);

    friend Mat4 operator-(const Mat4& a);
    friend Mat4 operator*(const Mat4& a, double s);
    friend Mat4 operator/(const Mat4& a, double s);
    friend Mat4 operator*(double s, const Mat4& a);
    friend Mat4 operator/(double s, const Mat4& a);

    friend Vec4 operator*(const Mat4& a, const Vec4& b);
    friend Vec4 operator*(const Vec4& a, const Mat4& b);

    /*----------------------------------------*/

    friend std::ostream& operator<<(std::ostream& stream, const Mat4& a);

    /*----------------------------------------*/

    double data[16];
};

