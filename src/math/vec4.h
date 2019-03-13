//============================================================================
// @name        : vec3.cpp
// @author      : Thomas Dooms
// @date        : 10/01/19
// @version     : 1.1
// @copyright   : BA1 Informatica - Thomas Dooms - University of Antwerp
// @description : math classes inspired by GLSL, uses ^ for elementwise multiplications.
//============================================================================

#pragma once

#include <iostream>
#include <vector>

#include "vec3.h"
#include "vec2.h"

struct Mat4;

struct Vec4
{
    Vec4() : data{0,0,0,0} {}
    Vec4(double x) : data{x,0,0,0} {}
    Vec4(double x, double y) : data{x,y,0,0} {}
    Vec4(double x, double y, double z) : data{x,y,z,0} {}
    Vec4(double x, double y, double z, double w) : data{x,y,z,w} {}

    Vec4(const std::vector<double>& data) : data{data[0], data[1], data[2], data[3]} {}
    Vec4(const Vec2& data) : data{data[0], data[1], 0, 1} {}
    Vec4(const Vec3& data) : data{data[0], data[1], data[2], 1} {}

    /*----------------------------------------*/

    double& operator[](int index) { return data[index]; }
    double operator[](int index) const { return data[index]; }

    const double* begin() const { return data; }
    const double* end() const { return data+3; }

    Vec3 xyz() const { return {data[0], data[1], data[2]}; }
    Vec2 xy()  const { return {data[0], data[1]}; }

    /*----------------------------------------*/

    Vec4& operator+=(const Vec4& v);
    Vec4& operator-=(const Vec4& v);
    Vec4& operator^=(const Vec4& v);
    Vec4& operator/=(const Vec4& v);

    Vec4& operator*=(const Mat4& m);
    Vec4& operator*=(double s);
    Vec4& operator/=(double s);

    /*----------------------------------------*/

    friend Vec4 operator+(const Vec4& a, const Vec4& b);
    friend Vec4 operator-(const Vec4& a, const Vec4& b);
    friend Vec4 operator^(const Vec4& a, const Vec4& b);
    friend Vec4 operator/(const Vec4& a, const Vec4& b);

    friend Vec4 operator-(const Vec4& a);
    friend Vec4 operator*(const Vec4& a, double s);
    friend Vec4 operator/(const Vec4& a, double s);
    friend Vec4 operator*(double s, const Vec4& a);
    friend Vec4 operator/(double s, const Vec4& a);

    /*----------------------------------------*/

    friend double dot(const Vec4& a, const Vec4& b);
    friend double norm(const Vec4& a);
    friend Vec4 normalize(const Vec4& a);
    friend Vec4& normalize(Vec4& a);

    friend std::ostream& operator<<(std::ostream& stream, const Vec4& a);

    /*----------------------------------------*/

    double data[4];
};

