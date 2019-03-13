//============================================================================
// @name        : vec2.h
// @author      : Thomas Dooms
// @date        : 10/01/19
// @version     : 1.1
// @copyright   : BA1 Informatica - Thomas Dooms - University of Antwerp
// @description : math classes inspired by GLSL, uses ^ for elementwise multiplications.
//============================================================================

#pragma once

#include <iostream>
#include <vector>
struct Mat4;

struct Vec2
{
    Vec2() : data{0,0} {}
    Vec2(double x) : data{x,0} {}
    Vec2(double x, double y) : data{x,y} {}

    Vec2(const std::vector<double>& data) : data{data[0], data[1]} {}

    /*----------------------------------------*/

    double& operator[](int index) { return data[index]; }
    double operator[](int index) const { return data[index]; }

    const double* begin() const { return data; }
    const double* end() const { return data+1; }

    /*----------------------------------------*/

    Vec2& operator+=(const Vec2& v);
    Vec2& operator-=(const Vec2& v);
    Vec2& operator^=(const Vec2& v);
    Vec2& operator/=(const Vec2& v);

    Vec2& operator*=(const Mat4& m);
    Vec2& operator*=(double s);
    Vec2& operator/=(double s);

    /*----------------------------------------*/

    friend Vec2 operator+(const Vec2& a, const Vec2& b);
    friend Vec2 operator-(const Vec2& a, const Vec2& b);
    friend Vec2 operator^(const Vec2& a, const Vec2& b);
    friend Vec2 operator/(const Vec2& a, const Vec2& b);

    friend Vec2 operator-(const Vec2& a);
    friend Vec2 operator*(const Vec2& a, double s);
    friend Vec2 operator/(const Vec2& a, double s);
    friend Vec2 operator*(double s, const Vec2& a);
    friend Vec2 operator/(double s, const Vec2& a);

    /*----------------------------------------*/

    friend double dot(const Vec2& a, const Vec2& b);
    friend double norm(const Vec2& a);
    friend Vec2 normalize(const Vec2& a);
    friend Vec2& normalize(Vec2& a);

    friend std::ostream& operator<<(std::ostream& stream, const Vec2& a);

    /*----------------------------------------*/

    double data[2];
};