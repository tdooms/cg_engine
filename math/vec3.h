//============================================================================
// @name        : vec3.h
// @author      : Thomas Dooms
// @date        : 10/01/19
// @version     : 1.1
// @copyright   : BA1 Informatica - Thomas Dooms - University of Antwerp
// @description : math classes inspired by GLSL, uses ^ for elementwise multiplications.
//============================================================================

#pragma once

#include <iostream>
#include <vector>

#include "vec2.h"

struct mat4;

struct vec3
{
    vec3() : data{0,0,0} {}
    vec3(double x) : data{x,0,0} {}
    vec3(double x, double y) : data{x,y,0} {}
    vec3(double x, double y, double z) : data{x,y,z} {}
    vec3(const std::vector<double>& data) : data{data[0], data[1], data[2]} {}

    vec3(const vec2& data) : data{data[0], data[1], 0} {}

    /*----------------------------------------*/

    double& operator[](int index) { return data[index]; }
    double operator[](int index) const { return data[index]; }

    const double* begin() const { return data; }
    const double* end() const { return data+2; }

    vec2 xy()  const { return {data[0], data[1]}; }

    /*----------------------------------------*/

    vec3& operator+=(const vec3& v);
    vec3& operator-=(const vec3& v);
    vec3& operator^=(const vec3& v);
    vec3& operator/=(const vec3& v);

    vec3& operator*=(const mat4& m);
    vec3& operator*=(double s);
    vec3& operator/=(double s);

    /*----------------------------------------*/

    friend vec3 operator+(const vec3& a, const vec3& b);
    friend vec3 operator-(const vec3& a, const vec3& b);
    friend vec3 operator^(const vec3& a, const vec3& b);
    friend vec3 operator/(const vec3& a, const vec3& b);

    friend vec3 operator-(const vec3& a);
    friend vec3 operator*(const vec3& a, double s);
    friend vec3 operator/(const vec3& a, double s);
    friend vec3 operator*(double s, const vec3& a);
    friend vec3 operator/(double s, const vec3& a);

    /*----------------------------------------*/

    friend vec3 cross(const vec3& a, const vec3& b);
    friend double dot(const vec3& a, const vec3& b);
    friend double norm(const vec3& a);
    friend vec3 normalize(const vec3& a);
    friend vec3& normalize(vec3& a);

    friend std::ostream& operator<<(std::ostream& stream, const vec3& a);

    /*----------------------------------------*/

    double data[3];
};