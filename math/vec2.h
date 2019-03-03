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
struct mat4;

struct vec2
{
    vec2() : data{0,0} {}
    vec2(double x) : data{x,0} {}
    vec2(double x, double y) : data{x,y} {}
    vec2(const std::vector<double>& data) : data{data[0], data[1]} {}

    /*----------------------------------------*/

    double& operator[](int index) { return data[index]; }
    double operator[](int index) const { return data[index]; }

    const double* begin() const { return data; }
    const double* end() const { return data+1; }

    /*----------------------------------------*/

    vec2& operator+=(const vec2& v);
    vec2& operator-=(const vec2& v);
    vec2& operator^=(const vec2& v);
    vec2& operator/=(const vec2& v);

    vec2& operator*=(const mat4& m);
    vec2& operator*=(double s);
    vec2& operator/=(double s);

    /*----------------------------------------*/

    friend vec2 operator+(const vec2& a, const vec2& b);
    friend vec2 operator-(const vec2& a, const vec2& b);
    friend vec2 operator^(const vec2& a, const vec2& b);
    friend vec2 operator/(const vec2& a, const vec2& b);

    friend vec2 operator-(const vec2& a);
    friend vec2 operator*(const vec2& a, double s);
    friend vec2 operator/(const vec2& a, double s);
    friend vec2 operator*(double s, const vec2& a);
    friend vec2 operator/(double s, const vec2& a);

    /*----------------------------------------*/

    friend double dot(const vec2& a, const vec2& b);
    friend double norm(const vec2& a);
    friend vec2 normalize(const vec2& a);
    friend vec2& normalize(vec2& a);

    friend std::ostream& operator<<(std::ostream& stream, const vec2& a);

    /*----------------------------------------*/

    double data[2];
};