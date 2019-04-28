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

struct Mat4;

struct Vec3
{
    Vec3() : data{0,0,0} {}
    Vec3(double x) : data{x,0,0} {}
    Vec3(double x, double y) : data{x,y,0} {}
    Vec3(double x, double y, double z) : data{x,y,z} {}

    Vec3(const std::vector<double>& data) : data{data[0], data[1], data[2]} {}
    Vec3(const Vec2& data) : data{data[0], data[1], 0} {}

    /*----------------------------------------*/

    double& operator[](int index) { return data[index]; }
    double operator[](int index) const { return data[index]; }

    const double* begin() const { return data; }
    const double* end() const { return data+2; }

    Vec2 xy() const { return {data[0], data[1]}; }

    /*----------------------------------------*/

    Vec3& operator+=(const Vec3& v);
    Vec3& operator-=(const Vec3& v);
    Vec3& operator^=(const Vec3& v);
    Vec3& operator/=(const Vec3& v);

    Vec3& operator*=(const Mat4& m);
    Vec3& operator*=(double s);
    Vec3& operator/=(double s);

    /*----------------------------------------*/

    friend Vec3 operator+(const Vec3& a, const Vec3& b);
    friend Vec3 operator-(const Vec3& a, const Vec3& b);
    friend Vec3 operator^(const Vec3& a, const Vec3& b);
    friend Vec3 operator/(const Vec3& a, const Vec3& b);

    friend Vec3 operator-(const Vec3& a);
    friend Vec3 operator*(const Vec3& a, double s);
    friend Vec3 operator/(const Vec3& a, double s);
    friend Vec3 operator*(double s, const Vec3& a);
    friend Vec3 operator/(double s, const Vec3& a);

    friend bool operator<(const Vec3& a, const Vec3& b);
    friend bool operator>(const Vec3& a, const Vec3& b);

    /*----------------------------------------*/

    friend Vec3 cross(const Vec3& a, const Vec3& b);
    friend double dot(const Vec3& a, const Vec3& b);
    friend double norm(const Vec3& a);
    friend Vec3 normalize(const Vec3& a);
    friend Vec3& normalize(Vec3& a);

    friend std::ostream& operator<<(std::ostream& stream, const Vec3& a);

    /*----------------------------------------*/

    double data[3];
};