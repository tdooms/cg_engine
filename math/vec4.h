//
// Created by Thomas on 10/01/2019.
//

#pragma once

#include <iostream>
struct mat4;

struct vec4
{
    vec4() : data{0,0,0,1} {}
    vec4(double x) : data{x,0,0,1} {}
    vec4(double x, double y) : data{x,y,0,1} {}
    vec4(double x, double y, double z) : data{x,y,z,1} {}
    vec4(double x, double y, double z, double w) : data{x,y,z,w} {}

    /*----------------------------------------*/

    double& operator[](int index) { return data[index]; }
    double operator[](int index) const { return data[index]; }

    const double* begin() const { return data; }
    const double* end() const { return data+3; }

    double getX() const { return data[0]; }
    double getY() const { return data[1]; }
    double getZ() const { return data[2]; }
    double getW() const { return data[3]; }

    double& getX() { return data[0]; }
    double& getY() { return data[1]; }
    double& getZ() { return data[2]; }
    double& getW() { return data[3]; }

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

    friend vec4 cross(const vec4& a, const vec4& b);
    friend double dot(const vec4& a, const vec4& b);
    friend double norm(const vec4& a);
    friend vec4 normalize(const vec4& a);
    friend vec4& normalize(vec4& a);

    friend std::ostream& operator<<(std::ostream& stream, const vec4& a);

    /*----------------------------------------*/

    double data[4];
};

