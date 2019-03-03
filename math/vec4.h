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

struct mat4;

struct vec4
{
    vec4() : data{0,0,0,0} {}
    vec4(double x) : data{x,0,0,0} {}
    vec4(double x, double y) : data{x,y,0,0} {}
    vec4(double x, double y, double z) : data{x,y,z,0} {}
    vec4(double x, double y, double z, double w) : data{x,y,z,w} {}
    vec4(const std::vector<double>& data) : data{data[0], data[1], data[2], data[3]} {}

    vec4(const vec2& data) : data{data[0], data[1], 0, 1} {}
    vec4(const vec3& data) : data{data[0], data[1], data[2], 1} {}

    /*----------------------------------------*/

    double& operator[](int index) { return data[index]; }
    double operator[](int index) const { return data[index]; }

    const double* begin() const { return data; }
    const double* end() const { return data+3; }

    vec3 xyz() const { return {data[0], data[1], data[2]}; }
    vec2 xy()  const { return {data[0], data[1]}; }

    /*----------------------------------------*/

    vec4& operator+=(const vec4& v);
    vec4& operator-=(const vec4& v);
    vec4& operator^=(const vec4& v);
    vec4& operator/=(const vec4& v);

    vec4& operator*=(const mat4& m);
    vec4& operator*=(double s);
    vec4& operator/=(double s);

    /*----------------------------------------*/

    friend vec4 operator+(const vec4& a, const vec4& b);
    friend vec4 operator-(const vec4& a, const vec4& b);
    friend vec4 operator^(const vec4& a, const vec4& b);
    friend vec4 operator/(const vec4& a, const vec4& b);

    friend vec4 operator-(const vec4& a);
    friend vec4 operator*(const vec4& a, double s);
    friend vec4 operator/(const vec4& a, double s);
    friend vec4 operator*(double s, const vec4& a);
    friend vec4 operator/(double s, const vec4& a);

    /*----------------------------------------*/

    friend double dot(const vec4& a, const vec4& b);
    friend double norm(const vec4& a);
    friend vec4 normalize(const vec4& a);
    friend vec4& normalize(vec4& a);

    friend std::ostream& operator<<(std::ostream& stream, const vec4& a);

    /*----------------------------------------*/

    double data[4];
};

